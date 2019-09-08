#ifndef __FILEPATH_H
#define __FILEPATH_H

#define FPATH_FILE "/kernel/lib/filepath"

/*
 * Resolves a path (resolves ~(home) ..(parent) and .(this) directories)
 * Example:  path = "~manwe/room/./obj/../file.c"
 *               => "/home/manwe/room/file.c"
 */
#define RPATH(path) call_other(FPATH_FILE, "resolve_path", path)

/*
 * Resolves and creates a composite path from a path and subpath
 * If subpath starts with '/' then path is ignored.
 * Example: path = "/d/Domain/coder", name = "below/eastend.c"
 *              => "/d/Domain/coder/below/eastend.c"
 */
#define FPATH(path, subpath) call_other(FPATH_FILE, "fix_path", path, subpath)

/*
 * Resolves and reduces a path to its tilde counterpart
 * Example:  path = "/home/manwe/./room/file.c"
 *               => "~manwe/room/file.c"
 */
#define TPATH(path) call_other(FPATH_FILE, "tilde_path", path)

#endif
