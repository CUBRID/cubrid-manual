#!/usr/local/bin/python
# SQL codes of release_note are not extracted by this script.
# run this program on the top of the directories that you want to extract sql codes.

import glob, os.path, re
ndir = nfile = 0

def traverse(dir, depth):
	global ndir, nfile,sqlf
	for obj in glob.glob(dir + '/*'):
		if os.path.isdir(obj):
			if  os.path.basename(obj).find('release_note') == 0:
				continue
			else:
				ndir += 1
				traverse(obj, depth + 1)
		elif os.path.isfile(obj):
			nfile += 1
			#if not ((obj.rfind('.rst') == -1) and (obj.rfind('.inc')) == -1):
			if ((obj.endswith('.rst') == 1) or (obj.endswith('.inc')) == 1):
				f = open(os.path.abspath(obj), 'r')
				dname = os.path.dirname(os.path.abspath(obj))

				str_array = f.readlines();
				
				in_block_flag = 0
				pat = ''
				loop = 0

				# diff
				fname = os.path.basename(obj) + '.sql'
				absname = dname + '/' + fname
				absname_src = dname.replace('RB-9.2.0','RB-8.4.4') + '/' + fname
				print "echo '>> diff " + absname_src + '.out ' + absname + '.out <<\'\n'
				print "diff " + absname_src + '.out ' + absname + '.out\n'
				print "echo '>> diff " + absname_src + '.err ' + absname + '.err <<\'\n'
				print "diff " + absname_src + '.err ' + absname + '.err\n'

				for string in str_array:
					i = string.find('.. code-block:: sql')
					if i >= 0:
						in_block_flag = 1
						loop = loop + 1

						N = i + 4
						pat = ' {' + str(N) + '}'
						continue
					elif in_block_flag == 1:
						if string.isspace():
							continue;
						elif bool(re.match(pat, string)):
							continue;
						else:
							# end of block
							in_block_flag = 0
				f.close()
					
		else:
			print(prefix + 'unknown object :', obj)

if __name__ == '__main__':
	traverse('.', 0)

