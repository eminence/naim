/*  ___  ___  ___ ___
** / __|/ _ \/ __/ __| secs
** \__ \  __/ (__\__ \ Copyright 1999-2003 Daniel Reed <n@ml.org>
** |___/\___|\___|___/ Simple Embedded Client Scripting
*/
#include <naim/secs.h>

extern int	(*secs_client_cmdhandler)(const char *);

struct {
	secs_block_t	*secs_root;
} sd;

secs_block_t	*secs_block_getroot(void) {
	return(sd.secs_root);
}

int	secs_init(void) {
	secs_mem_init();
	secs_var_init();
	secs_block_init();
	secs_script_init();
	sd.secs_root = secs_block_create(NULL, "root");
	assert(sd.secs_root != NULL);
	return(1);
}

void	secs_handle(char *line) {
	(*secs_client_cmdhandler)(line);
}

int	secs_makevar(const char *name, const char *value, char type) {
	secs_var_t	*var = NULL;

	if ((name == NULL) || (value == NULL) || (type == 0))
		return(secs_var_set(var, value));

	if ((var = secs_var_find(NULL, name)) != NULL)
		return(0);

	if (toupper(type) == 'I')
		var = secs_var_create(name, secs_var_set_int);
	else if (toupper(type) == 'S')
		var = secs_var_create(name, secs_var_set_str);
	else if (toupper(type) == 'B')
		var = secs_var_create(name, secs_var_set_switch);
	else
		return(0);
	secs_var_set(var, value);
	return(secs_block_var_add(sd.secs_root, var));
}

int	secs_setvar(const char *name, const char *val) {
	secs_var_t	*var = NULL;

	if ((name == NULL) || (val == NULL))
		return(0);
	if ((var = secs_var_find(NULL, name)) == NULL)
		return(secs_makevar(name, val, 'S'));
	return(secs_var_set(var, val));
}

char	*secs_getvar(const char *name) {
	secs_var_t	*var = NULL;

	if ((var = secs_var_find(NULL, name)) == NULL)
		return(NULL);
	return(var->val);
}

int	secs_getvar_int(const char *name) {
	char	*p = secs_getvar(name);

	if (p != NULL)
		return(atoi(p));
	return(0);
}

#define var (*((secs_var_t **)_var))
char	*secs_listvars(int i, size_t *length, void **_var) {
	char	*tmp = NULL;

	if (i == 0) {
		var = sd.secs_root->variables;
		return(NULL);
	}
	if (var == NULL)
		return(NULL);
	tmp = var->name;
	if (length != NULL)
		*length = var->length;
	var = var->next;
	return(tmp);
}
