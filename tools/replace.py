#!/usr/bin/env python
# Replaces <text1> by <text2> recursively in <dir>
# Usage replace.py <dir> <text1> <text2>

import sys
import os, os.path

if len(sys.argv) <> 4:
	print 'Usage: replace.py <dir> <text1> <text2>'
	sys.exit(1)

dir1 = sys.argv[1]
text1 = sys.argv[2]
text2 = sys.argv[3]


def replace_text(pth):
	"""Replace every occurrence of 'text1'
	with text2 in file-path 'pth'"""
	fil = file(pth, 'r')
	contents = fil.read()
	fil.close()
	del fil
	fil = file(pth, 'w')
	fil.write(contents.replace(text1, text2))
	fil.close()
	del fil


for root, dirs, files in os.walk(dir1):
	for fil in files:
		pth = os.path.join(root, fil)
		print 'Replacing', pth, '...',
		try:
			replace_text(pth)
		except Exception, msg:
			sys.stderr.write('\nError replacing text: %s' % msg)
			print 'error'
		else:
			print 'done'
