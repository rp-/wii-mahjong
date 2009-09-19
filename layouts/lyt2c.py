'''
Created on Sep 2, 2009

@author: R.Peinthor
'''
import sys

def main(argv):
    createHFile( 'mahjongglayouts.h', argv)
    createCFile( 'mahjongglayouts.c', argv)

def createHFile(hfilename, files):
  hfile = open( hfilename, 'w+')
  hfile.write('#ifndef _mahjongglayouts_h_\n')
  hfile.write('#define _mahjongglayouts_h_\n\n')
  hfile.write('#include <ogcsys.h> //for u8\n\n')
  for filename in files:
    hfile.write('extern const u8 %s_mtl[];\n' % getFilename(filename))
  hfile.write('\n#endif\n')
  hfile.close()

def createCFile( cfilename, files):
  cfile = open( cfilename, 'w+')
  cfile.write( '#include "mahjongglayouts.h"\n')
  for filename in files:
    field = [ [ 0 for x in range(29)] for y in range(15) ]
    x = 0
    fd = open(filename, 'r')
    for line in fd:
      line = line.strip('\r\n\t');
      y = 0
      for char in line:
        if char != '-':
          field[x][y] = int(char)
        y += 1
      x += 1
    fd.close()
    cfile.write( '\nconst u8 %s_mtl[] = {\n' % getFilename(filename) )
    count = 0
    zfield = [ [ [ 0 for x in range(29)] for y in range(15) ] for z in range(5) ]
    for y in range(15):
      for x in range(29):
        for z in range(field[y][x]):
          zValue = firstFree(zfield, x, y, z)
          zfield[zValue][y][x] = 1
            
    for y in range(15):
      for x in range(29):
        for z in range(5):
          if zfield[z][y][x] == 1:
            cfile.write( '  %2d, %2d, %2d,\n' % (x, y, z) )
            count += 1
    cfile.write( '}; //%d tiles\n' % count )
  cfile.close()

def getFilename(filename):
  dotpos = filename.rfind('.')
  if filename.rfind('/') > -1:
    return filename[filename.rfind('/') : dotpos]
  elif filename.rfind('\\') > -1:
      return filename[filename.rfind('\\') :dotpos]
  else:
    return filename[:dotpos]

def firstFree(zfield, x, y, zorig):
    addToZ = 0
    if x > 0 and y > 0:
      z = 0
      while z < 5:
        if zfield[z][y][x] == 0 and zfield[z][y - 1][x] == 0 and zfield[z][y - 1][x - 1] == 0 and zfield[z][y][x - 1] == 0:
            addToZ = z
            z = 5
        z += 1
    else:
      addToZ = zorig
    return addToZ

if __name__ == "__main__":
    main(sys.argv[1:])

