#ifndef Q_CVAR_H
#define Q_CVAR_H

#if defined(__cplusplus)
extern "C" {
#endif

#define CVAR_ARCHIVE        1       // save the variable on change
#define CVAR_USERINFO       2       // added to userinfo when changed
#define CVAR_SERVERINFO     4       // added to serverinfo when changed
#define CVAR_NOSET          8       // don't all change from console
#define CVAR_LATCH          16      // save until map restart
#define CVAR_LATCH_VIDEO    32      // save until video restart
#define CVAR_LATCH_SOUND    64      // save until video restart
#define CVAR_CHEAT          128     // reset to default unless cheats are enabled
#define CVAR_READONLY       256     // user cannot change.
#define CVAR_DEVELOPER      512     // allow changing in dev builds; hidden in release

typedef int cvar_flag_t;

struct cvar_s
{
    char *name;
    char *string;
    char *dvalue;
    char *latched_string;
    cvar_flag_t flags;
    bool modified;
    float value;
    int integer;
};

typedef struct cvar_s cvar_t;

#endif

#ifndef CVAR_H
#define CVAR_H

static inline cvar_flag_t Cvar_FlagSet(cvar_flag_t *flags, cvar_flag_t flag);
static inline cvar_flag_t Cvar_FlagUnset(cvar_flag_t *flags, cvar_flag_t flag);
static inline cvar_flag_t Cvar_FlagClear(cvar_flag_t *flags);
static inline bool        Cvar_FlagIsSet(cvar_flag_t flags, cvar_flag_t flag);

static inline const char * Cvar_GetName(const cvar_t *var);
static inline const char * Cvar_GetLatchedString(const cvar_t *var);
static inline const char * Cvar_GetDefaultValue(const cvar_t *var);
static inline const char * Cvar_GetStringValue(const cvar_t *var);
static inline float        Cvar_GetFloatValue(const cvar_t *var);
static inline cvar_flag_t  Cvar_GetFlags(const cvar_t *var);

static inline bool      Cvar_CheatsAllowed(void);
static inline bool      Cvar_IsModified(const cvar_t *var);
static inline void      Cvar_SetModified(cvar_t *var);
static inline void      Cvar_Unset_Modified(cvar_t *var);


cvar_t * Cvar_Get(const char *name, const char *value, cvar_flag_t flags);
cvar_t * Cvar_Set(const char *name, const char *value);
cvar_t * Cvar_ForceSet(const char *name, const char *value);
cvar_t * Cvar_FullSet(const char *name, const char *value, cvar_flag_t flags, bool overwrite_flags);

void         Cvar_SetValue(const char *name, float value);
float        Cvar_Value(const char *name);
const char * Cvar_String(const char *name);
int          Cvar_Integer(const char *name);

//cvar_t * Cvar_Get(const char *name, const char *value, cvar_flag_t flags);
//cvar_t * Cvar_ForceSet(const char *name, const char *value);
//cvar_t * Cvar_Set(const char *name, const char *value);
//cvar_t * Cvar_FullSet(const char *name, const char *value, cvar_flag_t flags, bool overwrite);
cvar_t * Cvar_Find(const char *name);

char *Cvar_UserInfo(void);
char *Cvar_ServerInfo(void);

void Cvar_GetLatchedVars(cvar_flag_t flags);
void Cvar_FixCheatVars(void);

void Cvar_PreInit(void);
void Cvar_Init(void);
void Cvar_Shutdown(void);


// 
// inlined function implementations
//
static inline bool Cvar_CheatsAllowed(void)
{
    return Cvar_Value("g_cheats");
}
static inline const char *Cvar_GetName(const cvar_t *var)
{
    return var->name;
}
static inline const char *Cvar_GetLatchedString(const cvar_t *var)
{
    return var->latched_string;
}
static inline const char *Cvar_GetDefaultValue(const cvar_t *var)
{
    return var->dvalue;
}
static inline const char *Cvar_GetStringValue(const cvar_t *var)
{
    return var->string;
}
static inline float Cvar_GetFloatValue(const cvar_t *var)
{
    return var->value;
}
static inline int Cvar_GetIntegerValue(const cvar_t *var)
{
    return var->integer;
}
static inline cvar_flag_t Cvar_GetFlags(const cvar_t *var)
{
    return var->flags;
}
static inline bool Cvar_IsModified(const cvar_t *var)
{
    return var->modified;
}
static inline void Cvar_SetModified(cvar_t *var)
{
    var->modified = true;
}
static inline void Cvar_UnsetModified(cvar_t *var)
{
    var->modified = false;
}
static inline cvar_flag_t Cvar_FlagSet(cvar_flag_t *flags, cvar_flag_t flag)
{
    return *flags |= flag;
}
static inline cvar_flag_t Cvar_FlagUnset(cvar_flag_t *flags, cvar_flag_t flag)
{
    return *flags &= ~flag;
}
static inline cvar_flag_t Cvar_FlagsClear(cvar_flag_t *flags)
{
    return *flags = 0;
}
static inline bool Cvar_FlagIsSet(cvar_flag_t flags, cvar_flag_t flag)
{
    return (bool)((flags & flag) != 0);
}

#if defined(__cplusplus)
}
#endif
#endif // CVAR_H
