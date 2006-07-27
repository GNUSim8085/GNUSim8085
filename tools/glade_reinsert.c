/* interface.c patch */
  if (!pathname)
    {
	  /* temporaray patch
		 should be reinserted everytime glade overwrites this file
		 the code can be found in tools/glade_reinsert.c */
	  GString *str;
	  str = g_string_new(PACKAGE_PIXMAPS_DIR);
	  g_string_append(str, "/");
	  g_string_append(str, filename);
	  pathname = g_strdup(str->str);
	  g_string_free(str, TRUE);
      //g_warning (_("Couldn't find pixmap file: %s"), filename);
      //return NULL;
    }

	NOte: also change
gnusim8085/gnusim8085_icon.png
	to
gnusim8085_icon.png
	in interface.c
