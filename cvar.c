#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include <pthread.h>

#include <math.h>

#include "cvar.h"
#include "glob.h"
#include "q_trie.h"


static bool s_cvar_initialized = false;
static bool s_cvar_preinitialized = false;
static pthread_mutex_t s_cvar_mutex;

static trie_t * s_cvar_trie = NULL;
#define CVAR_TRIE_CASING    TRIE_CASE_INSENSITIVE


static int
Cvar_HasFlags(void *cvar, void *flags)
{
    assert(cvar);
    assert(flags);
    return Cvar_FlagIsSet(((cvar_t*)cvar)->flags, *(cvar_flag_t*)flags);
}

static int
Cvar_IsLatched(void *cvar, void *flags)
{
    const cvar_t* const var = (cvar_t*)cvar;
    assert(cvar);
    return Cvar_FlagIsSet(var->flags, *(cvar_flag_t*)flags) && var->latched_string;
}

static bool
Cvar_PatternMatches(void *cvar, void *pattern) 
{
    return (bool)(!pattern || glob_match((const char*)pattern, ((cvar_t*)cvar)->name, false));
}

cvar_t *
Cvar_Find(const char *var_name)
{
    cvar_t *var;
    assert(s_cvar_trie);
    pthread_mutex_lock(&s_cvar_mutex);
    Trie_Find(s_cvar_trie, var_name, TRIE_EXACT_MATCH, (void**)&var);
    pthread_mutex_unlock(&s_cvar_mutex);
    return var;
}

float
Cvar_Value(const char *var_name)
{
    const cvar_t * const var = Cvar_Find(var_name);
    return var ? atof(var->string) : 0.0f;
}

const char *
Cvar_String(const char *var_name)
{
    const cvar_t * const var = Cvar_Find(var_name);
    return var ? var->string : "";
}

int
Cvar_Integer(const char *var_name)
{
    const cvar_t * const var = Cvar_Find(var_name);
    return var ? var->integer : 0;
}

cvar_t *
Cvar_Get(const char *var_name, const char *var_value, cvar_flag_t flags)
{
    size_t tmpsiz;
    cvar_t * var;

    assert(s_cvar_trie);
    if (!var_name || !var_name[0])
        return NULL;

    if (!var_value)
        return NULL;

    pthread_mutex_lock(&s_cvar_mutex);
    Trie_Find(s_cvar_trie, var_name, TRIE_EXACT_MATCH, (void**)&var);
    pthread_mutex_unlock(&s_cvar_mutex);

    //
    // Update and return a pre-existing cvar
    //
    if ( var )
    {
        if (!var->dvalue || strcmp(var->dvalue, var_value))
        {
            if (var->dvalue)
            {
                free(var->dvalue);
                var->dvalue = NULL;
            }

            var->dvalue = strdup(var_value);
            if (var->dvalue == NULL)
                return NULL;
        }

        if (Cvar_FlagIsSet(flags, CVAR_READONLY)
         || Cvar_FlagIsSet(flags, CVAR_DEVELOPER))
        {
            if (!var->string || strcmp(var->string, var_value))
            {
                if (var->string)
                {
                    free(var->string);
                    var->string = NULL;
                }

                var->string = strdup(var_value);
                if (var->string == NULL)
                    return NULL;
                var->value = atof(var->string);
                var->integer = rint(var->value);
            }
            var->flags = flags;
        }

        Cvar_FlagSet(&var->flags, flags);
        return var;
    }
    
    //
    // Create a new cvar variable
    //
    
    tmpsiz = sizeof(*var)
           + strlen(var_name)
           + 1;

    var = calloc(1, tmpsiz);
    var->name = (char*)((uint8_t*)var + sizeof(*var));
    strcpy(var->name, var_name);
    var->dvalue = strdup((char*)var_value);
    var->string = strdup((char*)var_value);
    var->value = atof(var->string);
    var->integer = rint(var->value);
    var->flags = flags;

    pthread_mutex_lock(&s_cvar_mutex);
    Trie_Insert(s_cvar_trie, var_name, var);
    pthread_mutex_unlock(&s_cvar_mutex);

    return var;
}

static cvar_t *
Cvar_Set2(const char *var_name, const char *value, bool force)
{
    cvar_t *var = Cvar_Find(var_name);

    if (!var)
    {
        return Cvar_Get(var_name, value, 0);
    }

    if (!force)
    {
        if (Cvar_FlagIsSet(var->flags, CVAR_NOSET)
         || Cvar_FlagIsSet(var->flags, CVAR_READONLY)
         || Cvar_FlagIsSet(var->flags, CVAR_DEVELOPER))
        {
            printf("%s is write protected.\n", var_name);
            return var;
        }

        if (Cvar_FlagIsSet(var->flags, CVAR_CHEAT)
         && strcmp(value, var->dvalue))
        {
            if (!Cvar_CheatsAllowed())
            {
                printf("%s is cheat protected.\n", var_name);
                return var;
            }
        }
    }
    else
    {
        if (var->latched_string)
        {
            free(var->latched_string);
            var->latched_string = NULL;
        }
    }

    if (!strcmp(value, var->string))
        return var;

    free(var->string);
    var->string = strdup(value);
    var->value = atof(var->string);
    var->integer = rint(var->value);

    return var;
}

cvar_t *
Cvar_ForceSet(const char *var_name, const char *value)
{
    return Cvar_Set2(var_name, value, true);
}

cvar_t *
Cvar_Set(const char *var_name, const char *value)
{
    return Cvar_Set2(var_name, value, false);
}

cvar_t *
Cvar_FullSet(const char *var_name, const char *value, cvar_flag_t flags, bool overwrite)
{
    cvar_t *var;

    var = Cvar_Find(var_name);
    if (!var)
        return Cvar_Get(var_name, value, flags);

    if (overwrite)
    {
        var->flags = flags;
    }
    else
    {
        Cvar_FlagSet(&var->flags, flags);
    }

    return Cvar_Set2(var_name, value, overwrite);
}

void Cvar_SetValue(const char *var_name, float value)
{
    char val[32];
    
    if (value == rint(value))
        snprintf(val, sizeof val, "%d", (int)rint(value));
    else
        snprintf(val, sizeof val, "%f", value);

    Cvar_Set(var_name, val);
}

/* 
 * Any variables with CVAR_LATCHED will now be updated
 */
void Cvar_GetLatchedVars(cvar_flag_t flags)
{
    assert(s_cvar_trie);

    unsigned int i;
    struct trie_dump_s *dump = NULL;
    cvar_flag_t latchFlags;

    Cvar_FlagsClear(&latchFlags);
    Cvar_FlagSet(&latchFlags, CVAR_LATCH);
    Cvar_FlagUnset(&flags, ~latchFlags);

    if (!flags)
        return;

    pthread_mutex_lock(&s_cvar_mutex);
    Trie_DumpIf(s_cvar_trie, "", TRIE_DUMP_VALUES, Cvar_IsLatched, &flags, &dump);
    pthread_mutex_unlock(&s_cvar_mutex);

    for (i=0; i < dump->size; ++i)
    {
        cvar_t * const var = (cvar_t*)dump->key_value_vector[i].value;
        free(var->string);
        var->string = var->latched_string;
        var->latched_string = NULL;
        var->value = atof(var->string);
        var->integer = rint(var->value);
    }

    Trie_FreeDump(dump);
}

/*
 * All cheat variables will be reset to default unless cheats are enabled.
 */
void Cvar_FixCheatVars(void)
{
    assert(s_cvar_trie);

    struct trie_dump_s *dump = NULL;
    unsigned int i;
    cvar_flag_t flags = CVAR_CHEAT;

    if (Cvar_CheatsAllowed())
        return;

    pthread_mutex_lock(&s_cvar_mutex);
    Trie_DumpIf(s_cvar_trie, "", TRIE_DUMP_VALUES, Cvar_HasFlags, &flags, &dump);
    pthread_mutex_unlock(&s_cvar_mutex);

    for (i=0; i < dump->size; ++i)
    {
        cvar_t * const var = (cvar_t*)dump->key_value_vector[i].value;
        free(var->string);
        var->string = strdup(var->dvalue);
        var->value = atof(var->string);
        var->integer = rint(var->value);
    }
    Trie_FreeDump(dump);
}


void Cvar_PreInit(void)
{
    assert(!s_cvar_initialized);
    assert(!s_cvar_preinitialized);
    assert(!s_cvar_trie);
    pthread_mutex_init(&s_cvar_mutex, NULL);
    Trie_Create(CVAR_TRIE_CASING, &s_cvar_trie);
    s_cvar_preinitialized = true;
}

void Cvar_Init(void)
{
    assert(!s_cvar_initialized);
    assert(s_cvar_preinitialized);
    assert(s_cvar_trie);
    // Cmd_AddCommand(`set', Cvar_Set_f)
    // Cmd_AddCommand(`toggle', Cvar_Toggle_f)
    // Cmd_AddCommand(`reset', Cvar_Reset_f)
    // Cmd_AddCommand(`cvarlist', Cvar_List_f)
    s_cvar_initialized = true;
}

void Cvar_Shutdown(void)
{
    struct trie_dump_s *dump;
    int i;

    assert(s_cvar_preinitialized);
    assert(s_cvar_initialized);
    assert(s_cvar_trie);

    pthread_mutex_lock(&s_cvar_mutex);
    Trie_Dump(s_cvar_trie, "", TRIE_DUMP_VALUES, &dump);
    pthread_mutex_unlock(&s_cvar_mutex);

    for( i = 0; i < dump->size; ++i ) {
         cvar_t *const var = dump->key_value_vector[ i ].value;

         if( var->string )
            free(var->string);
         if( var->dvalue )
            free(var->dvalue);
        free(var);
    }

    Trie_FreeDump( dump );
    s_cvar_initialized = false;

    pthread_mutex_lock(&s_cvar_mutex);
    Trie_Destroy(s_cvar_trie);
    pthread_mutex_unlock(&s_cvar_mutex);

    s_cvar_trie = NULL;
    s_cvar_preinitialized = false;

    pthread_mutex_destroy(&s_cvar_mutex);
}
