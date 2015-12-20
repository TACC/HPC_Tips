#!/usr/bin/env python
# -*- python -*-
from __future__ import print_function
from fnmatch    import fnmatch
import os, sys, re, platform, getpass, base64, argparse
import MySQLdb as mdb
import warnings
try:
  import configparser
except:
  import ConfigParser as configparser

warnings.filterwarnings("ignore", "Unknown table.*")

dividerPat = re.compile(r'^##\-\-*$')

class LD_TIPS(object):
  def __init__(self, confFn):
    self.__host   = None
    self.__user   = None
    self.__passwd = None
    self.__db     = None
    self.__conn   = None
    self.__confFn = confFn

  def __readFromUser(self):
    """ Ask user for database access info. (private) """

    self.__host   = raw_input("Database host:")
    self.__user   = raw_input("Database user:")
    self.__passwd = getpass.getpass("Database pass:")
    self.__db     = raw_input("Database name:")
    self.__writeConfig()

  def __writeConfig(self):
    config=configparser.ConfigParser()
    config.add_section("MYSQL")
    config.set("MYSQL","HOST",self.__host)
    config.set("MYSQL","USER",self.__user)
    config.set("MYSQL","PASSWD",base64.b64encode(self.__passwd))
    config.set("MYSQL","DB",self.__db)

    fn = self.__db + "_db.conf"

    f = open(fn,"w")
    config.write(f)
    f.close()

  def __readConfig(self):
    """ Read database access info from config file. (private)"""
    confFn = self.__confFn
    try:
      config=configparser.ConfigParser()
      config.read(confFn)
      self.__host    = config.get("MYSQL","HOST")
      self.__user    = config.get("MYSQL","USER")
      self.__passwd  = base64.b64decode(config.get("MYSQL","PASSWD"))
      self.__db      = config.get("MYSQL","DB")
    except configparser.NoOptionError as err:
      sys.stderr.write("\nCannot parse the config file\n")
      sys.stderr.write("Switch to user input mode...\n\n")
      self.__readFromUser()

  def db_connect(self):
    if(os.path.exists(self.__confFn)):
      self.__readConfig()
    else:
      self.__readFromUser()

    self.__conn = mdb.connect(self.__host, self.__user, self.__passwd, self.__db)

    with self.__conn:
      cur = self.__conn.cursor()
      cur.execute("DROP TABLE IF EXISTS tips")
      cur.execute("CREATE TABLE tips ( tips_id INT  PRIMARY KEY AUTO_INCREMENT, \
                   msg varchar(2048) not null)")

  def insertTip(self, txt):

    with self.__conn:
      cur = self.__conn.cursor()
      txt = txt.replace("'",r"\'")

      cur.execute("INSERT INTO tips(msg) VALUES('%s')" % txt)


  def db_disconnect(self):
    self.__conn.close()


def files_in_tree(path, pattern):
  fileA = []
  wd = os.getcwd()
  if (not os.path.isdir(path)):
    return fileA

  os.chdir(path)
  path = os.getcwd()
  os.chdir(wd)

  for root, dirs, files in os.walk(path):
    for name in files:
      fn = os.path.join(root, name)
      if (fnmatch(fn,pattern)):
        fileA.append(fn)
  fileA.sort()
  return fileA  

def main():

  confFn = "HPCTips_db.conf"
  

  myhost = platform.node()
  host   = (myhost == "tacc-stats") and "localhost" or "tacc-stats.tacc.utexas.edu"
  user   = "tipBuilder"
  passwd = "test623"
  db     = "HPCTips"

  tips   = LD_TIPS(confFn)

  tips.db_connect()

  fileA  = files_in_tree("./tips", "*.tips")

  for fn in fileA:
    f      = open (fn,"r")
    lines  = f.readlines()
    f.close()

    idx    = 0
    txt    = ""
    sA     = []
    for s in lines:
      s = s.rstrip()
      m = dividerPat.search(s)
      if (m and sA):
        txt = "\n".join(sA)
        idx = idx + 1

        tips.insertTip(txt)
        sA  = []
      elif (not m):
        sA.append(s)
  
  tips.db_disconnect()
  


if ( __name__ == '__main__'): main()
