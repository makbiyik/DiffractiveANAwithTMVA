#! /usr/bin/env python
#
import os

from optparse import OptionParser

parser = OptionParser()
parser.add_option("-s", "--source", dest="source_dir", default="/afs/cern.ch/work/m/makbiyik/public/Data_TMVA/",
                  help="SOURCE directory of analyzer output files", metavar="SOURCE")
parser.add_option("-t", "--target", dest="target_dir", default="./data",
                  help="TARGET directory to copy skimmed trees", metavar="TARGET")
parser.add_option("-u", "--user", dest="user_name",
                  help="USERNAME at lxplus", metavar="USERNAME")
parser.add_option("-f", "--files", dest="file_name", default="*.root",
                  help="FILES to copy from lxplus to local", metavar="FILES")
parser.add_option("-v", action="store_true", dest="verbose",
                  help="just show copy command NOT execute it")

(options, args) = parser.parse_args()

if not options.user_name:
  parser.error("NO user name was given")
else:
  user_name = options.user_name

source_dir = "/afs/cern.ch/work/m/makbiyik/public/Data_TMVA/"
target_dir = "./data/"
file_name = "*.root"

if options.source_dir: source_dir = options.source_dir
if options.target_dir: target_dir = options.target_dir
if options.file_name: file_name = options.file_name

copy_command = "scp " + user_name + "@lxplus.cern.ch:" + source_dir + file_name + " " + target_dir

if options.verbose:
  print copy_command
else:
  os.system(copy_command)