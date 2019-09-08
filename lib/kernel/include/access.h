# ifndef ACCESS_DEF
# define ACCESS_DEF

/* access data indices */
#define _ADATA_SNOOP    0
#define _ADATA_FORCE    1
#define _ADATA_FILE     2

/* access checking macros */
# define _ACCESS_LEVEL(euid)\
  _SYSTEMD->_Q_coder_level(euid)
# define _ACCESS_SETEUID(pobj, obj, id) \
  _ACCESSD->_Q_seteuid_access(pobj, obj, id)
# define _ACCESS_FORCE(forcer, target) \
  _ACCESSD->_Q_force_access(forcer, target)
# define _ACCESS_SNOOP(initiator, demon, target) \
  _ACCESSD->_Q_snoop_access(initiator, demon, target)
# define _ACCESS_READ(file, obj, func) \
  _ACCESSD->_Q_read_access(file, obj, func)
# define _ACCESS_WRITE(file, obj, func) \
  _ACCESSD->_Q_write_access(file, obj, func)
# define _ACCESS_DESTRUCT(obj, func) \
  _ACCESSD->_Q_destruct_access(obj, func)

#define _ACCESS_DEFAULT "default"

#endif
