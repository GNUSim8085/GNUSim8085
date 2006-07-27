/*
	Copyright (C) 2003  Sridhar <r_sridhar@users.sf.net>
	
	This file is part of GNUSim8085.

	GNUSim8085 is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	GNUSim8085 is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with GNUSim8085; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "asm-ds-symtable.h"
#include "asm-source.h"

GHashTable *hash_table = NULL;

static void
_asm_sym_delete_value (gpointer data)
{
	AsmSymEntry *entry = (AsmSymEntry *) data;

	g_free (entry);
}

/* init */
void
asm_sym_init (void)
{
	g_assert (hash_table == NULL);
	hash_table =
		g_hash_table_new_full (g_str_hash, g_str_equal, NULL,
				       _asm_sym_delete_value);

	g_assert (hash_table);
}

/* deinit */
gboolean
ret_true (gpointer key, gpointer value, gpointer user_data)
{
	return TRUE;
}

void
asm_sym_clear (void)
{
	g_assert (hash_table);

	g_hash_table_destroy (hash_table);
	hash_table = NULL;
	asm_sym_init ();
	//g_hash_table_foreach_remove (hash_table, ret_true, NULL);
}

/* clear sym table - not needed now 
void
asm_sym_clear (void)
{
	g_assert (hash_table);
	asm_sym_destroy ();
	asm_sym_init ();
}*/

static AsmSymEntry *
_asm_sym_entry_new (AsmSymType type, char *name, gpointer data,
		    gint no_of_data, gint lno)
{
	AsmSymEntry *entry;
	g_assert (name);

	entry = g_malloc (sizeof (AsmSymEntry));
	g_assert (entry);

	entry->type = type;
	g_stpcpy (entry->name, name);
	entry->data = data;
	entry->no_of_data = no_of_data;
	entry->listing_no = lno;

	return entry;
}

/* add symbols, data should be allocated by caller and deallocated by me
 * returns NULL if a symbol is redifined */
AsmSymEntry *
asm_sym_add (AsmSymType type, char *name, gpointer data, gint no_of_data,
	     gint listing_no)
{
	AsmSymEntry *entry;

	/* return if exists */
	if (g_hash_table_lookup (hash_table, name))
		return NULL;

	entry = _asm_sym_entry_new (type, name, data, no_of_data, listing_no);

	/* debug */
	g_hash_table_insert (hash_table, entry->name, entry);

	return entry;
}

/* query symbol */
AsmSymEntry *
asm_sym_query (char *name)
{
	return g_hash_table_lookup (hash_table, name);
}


/* scan symbol table */
void
asm_sym_scan (GHFunc cb, gpointer userdata)
{
	g_assert (cb);
	g_assert (hash_table);

	g_hash_table_foreach (hash_table, cb, userdata);
}

static guint _at_cb_mem;
static AsmSymType _at_cb_type;
static gchar *_at_cb_name = "ERR";
static void
_symbol_at_cb (gpointer key, gpointer value, gpointer user_data)
{
	AsmSymEntry *entry = (AsmSymEntry *)value;
	
	if ( entry->data == GUINT_TO_POINTER(_at_cb_mem) && entry->type == _at_cb_type )
		_at_cb_name = entry->name; //FIXME: how to inform caller to stop calling
	
}

gchar *
asm_sym_symbol_at (guint16 mem, AsmSymType type)
{
	_at_cb_mem = mem;
	_at_cb_type = type;
	
	//FIXME: this func will loop through ALL symbols always!
	g_hash_table_foreach (hash_table, _symbol_at_cb, NULL);
	return _at_cb_name;
}
