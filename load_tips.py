#!/usr/bin/env python
# -*- python -*-
from __future__ import print_function
import os, sys, re
import MySQLdb as mdb
import warnings
warnings.filterwarnings("ignore", "Unknown table.*")

dividerPat = re.compile(r'^##\-\-\-')

class LD_TIPS(object):
  def __init__(self, host, user, passwd, db):
    self.__conn   = None
    self.__host   = host
    self.__user   = user
    self.__passwd = passwd
    self.__db     = db
    
  def db_connect(self):
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


def main():

  host   = "localhost"
  user   = "tipBuilder"
  passwd = "test623"
  db     = "HPCTips"

  tips   = LD_TIPS(host, user, passwd, db)

  tips.db_connect()

  fn     = "approved_tips.txt"
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
    else:
      sA.append(s)
  
  tips.db_disconnect()
  


if ( __name__ == '__main__'): main()
