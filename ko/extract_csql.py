#!/usr/local/bin/python
# usage: ./extract_sql.py > file_name
# SQL codes of release_note are not extracted by this script.
# run this program on the top of the directories that you want to extract sql codes.

import glob, os.path, re
ndir = nfile = 0
# csql_str = "csql -u dba demodb -S -c\"";
csql_str = "csql -u dba demodb -e -c\"";
def traverse(dir, depth, loop):
	global ndir, nfile
	for obj in glob.glob(dir + '/*'):
		if os.path.isdir(obj):
			if  os.path.basename(obj).find('release_note') == 0:
				continue
			else:
				ndir += 1
				traverse(obj, depth + 1, loop)
		elif os.path.isfile(obj):
			nfile += 1
			if not ((obj.rfind('.rst') == -1) and (obj.rfind('.inc')) == -1):
				f = open(os.path.abspath(obj), 'r')
				print csql_str
				print('SELECT \'' + os.path.abspath(obj) + '\';' )
				print "\""
				str_array = f.readlines();

				in_block_flag = 0
				pat = ''
				for string in str_array:
					i = string.find('.. code-block:: sql')
					if i >= 0:
						in_block_flag = 1
						print csql_str
						loop = loop + 1
						print 'SELECT ' + str(loop) + ';'
						N = i + 4
						pat = ' {' + str(N) + '}'
						continue
					elif in_block_flag == 1:
						in_block_flag = 2
						continue
					elif in_block_flag == 2:
						if string.isspace():
							continue;
						elif bool(re.match(pat, string)):

							if (string.find('CREATE TABLE') > 0):
								out = re.split(";", re.sub(r"CREATE TABLE \b(([a-z]|[A-Z]|[0-9]|_)+)\b", r"DROP TABLE \1;", string))
								print out[0] + ";" 
							elif(string.find('CREATE VIEW') > 0):
								out = re.split(";", re.sub(r"CREATE VIEW \b(([a-z]|[A-Z]|[0-9]|_)+)\b", r"DROP VIEW \1;", string))
								print out[0] + ";" 
							elif(string.find('CREATE INDEX') > 0):
								out = re.split(";", re.sub(r"CREATE INDEX \b(([a-z]|[A-Z]|[0-9]|_)+)\b", r"DROP INDEX \1;", string))
								print out[0] + ";" 
							elif(string.find('CREATE TRIGGER') > 0):
								out = re.split(";", re.sub(r"CREATE TRIGGER \b(([a-z]|[A-Z]|[0-9]|_)+)\b", r"DROP TRIGGER \1;", string))
								print out[0] + ";" 
							elif(string.find('CREATE USER') > 0):
								out = re.split(";", re.sub(r"CREATE USER \b(([a-z]|[A-Z]|[0-9]|_)+)\b", r"DROP USER \1;", string))
								print out[0] + ";" 
							elif(string.find('CREATE SERIAL') > 0):
								out = re.split(";", re.sub(r"CREATE SERIAL \b(([a-z]|[A-Z]|[0-9]|_)+)\b", r"DROP SERIAL \1;", string))
								print out[0] + ";" 
							print string.strip().replace("\"","\\\"");

						else:
							# end of block
							in_block_flag = 0
							print "\""
				f.close()
				# end of file 
				if in_block_flag == 2:
					print "\""
					
		else:
			print(prefix + 'unknown object :', obj)

if __name__ == '__main__':
	traverse('.', 0, 0)

