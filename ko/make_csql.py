#!/usr/local/bin/python
# SQL codes of release_note are not extracted by this script.
# run this program on the top of the directories that you want to extract sql codes.

import glob, os.path, re
ndir = nfile = 0
# csql_str = "csql -u dba demodb -S -c\"";
#csql_str = "csql -u dba demodb -e -c\"";
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

				#print csql_str
				#print('SELECT \'' + os.path.abspath(obj) + '\';' )
				#print "\""
				str_array = f.readlines();
				
				in_block_flag = 0
				pat = ''
				loop = 0
				fname = os.path.basename(obj) + '.sql'
				absname = dname + '/' + fname
				print "csql -u dba -S -e -i " + absname + " -o " + absname + '.out' + ' demodb' + ' > ' + absname + '.err 2>&1\n'
				for string in str_array:
					i = string.find('.. code-block:: sql')
					if i >= 0:
						in_block_flag = 1
						#print csql_str
						loop = loop + 1
						#print 'SELECT ' + str(loop) + ';'

						N = i + 4
						pat = ' {' + str(N) + '}'
						continue
					#elif in_block_flag == 1:
					#	in_block_flag = 2
					#	continue
					#elif in_block_flag == 2:
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

