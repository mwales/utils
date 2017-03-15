#!/usr/bin/python

# Usage ./renamefiles.sh
#  Create a subdir thumbs
#  Convert filenames to prefix_hash.ext
#    Prefix preserves ordering
#  Create an smallhash.html linking everything

import os
import hashlib
import PIL
from PIL import Image

THUMBDIR = "/thumb"
THUMBWIDTH = 300

def fileHash(filename):
	f = open(filename, 'r')
	filecontents = f.read()
	hashcode = hashlib.md5()
	hashcode.update(filecontents)
	f.close()
	return hashcode.hexdigest()

def processFile(filename):
	md5hashhex = fileHash(filename)
	print "MD5 of " + filename + " is " + md5hashhex

	filePathParts = os.path.splitext(filename)
	if len(filePathParts) != 2:
		print "Error processing the filename " + filename
		return filename

	#print "Prefix = " + filePathParts[0]
	#print "Suffix = " + filePathParts[1]
	return filePathParts[0] + "_" + md5hashhex + filePathParts[1]

def createThumb(inputFilename, outputFilename):
	img = Image.open(inputFilename)
	scalingFactor = float(THUMBWIDTH) / float(img.size[0])
	newSize = ( int( float(img.size[0]) * scalingFactor), int( float(img.size[1]) * scalingFactor))
	thumbimg = img.resize(newSize)
	thumbimg.save(outputFilename) 

def isImageFile(filename):
	parts = os.path.splitext(filename)
	ext = parts[1].upper()
	# match for image filename extensions
	if (ext == ".JPG") or (ext == ".PNG") or \
           (ext == ".JPEG") or (ext == ".BMP") or \
           (ext == ".GIF"):
		return True
	else:
		return False

mycwd = os.getcwd()
print "CWD:" + mycwd
filelist = os.listdir(mycwd)

# Does a thumb folder exist?
THUMBDIR = "/thumb"
thumbexist = os.path.isdir(mycwd + THUMBDIR )
if not thumbexist:
	os.mkdir(mycwd + THUMBDIR)

# Generate the new filename for all of the files and create thumbnails
filenameMap = {}
for singleFile in filelist:
	if os.path.isfile(singleFile) and isImageFile(singleFile):
		hashedName = processFile(singleFile)
		print hashedName
		filenameMap[singleFile] = hashedName
		createThumb(singleFile, mycwd + THUMBDIR + "/th_" + hashedName)
	else:
		print "Ignoring directory / non-image " + singleFile

# Create the output page
html = open("index.html", "w")
html.write('<!DOCTYPE html>\n')
html.write('<head>\n')
html.write('   <meta name="robots" content="noindex" />\n')
html.write('</head>\n')
html.write('<html>\n')
html.write("<body>\n\n")

for orig in filenameMap:
	hashname = filenameMap[orig]
	os.rename(orig, hashname)
	html.write('   <a href="' + hashname + '">\n')
	html.write('      <img src="thumb/th_' + hashname + '" />\n')
	html.write('   </a>\n\n')

html.write("</body>\n")
html.write("</html>\n")
html.close()

htmlHash = fileHash("index.html")
os.rename("index.html", "index_" + htmlHash + ".html")
