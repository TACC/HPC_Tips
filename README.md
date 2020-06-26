HPC_Tips:
=========

HPC_Tips is a database for short tips to help HPC users.  This tool consists several parts:

(1) A program, **showTips**, is installed on login nodes which asks for one or more tips from the HPC_Tips database.

(2) A couple of files containing short tips each being 1 to 5 lines long.

(3) A python program to load the database from supplied list of tips

(4) Tools to setup the configuration file and build and install the necessary programs.

This works by installing a MySQL database on a server which is accessible from all your login nodes.  The source for this
project can live anywhere.  One uses the "**load_tips.py**" script to install the tips in the database. You place the source on a
host where you have write access to the database.  If you only allow write access to the database on the server, then the
source will have to live there.

Note that the tips database is independent of the install of the **showTips** program used to display tips to users.  Tips can be
updated at anytime.

Security Issues:
================

In order for a user to access the MySQL database, they must have a MySQL account and passwd.  All users
use the same account and password that is hardcoded into the **showTips** binary. A determined user might be
able to decode the **showTips** binary to exact the MySQL account and password.  However this account and password
only have "select" privileges.  They cannot delete or modify the database in any way.

The python script and MySQL username and password that can change the database need to be stored in a
location where users cannot access such as the server where the database is stored.

Runtime issues
==============

Since this **showTips** program is part of the login procedure, it shouldn't hang if the database cannot be accessed.
The program has 0.4 second timeout if the database cannot be accessed and no warning is produced normally.

To install:
===========

1) Setup MySQL Database:

Have a server which can be seen on the network from your login nodes.  On that server install a mysql database server and allow
your login nodes access to the the mysql ports on this server. This might require opening ports in various firewalls.  For the
purposes of this discussion, lets assume that this server is called "alice".

Once this mysql database server is running you'll need to create two accounts to access this database. Using "tipBuilder" as
the account name that has write access to the database and setting "test623" as the password.  A second account (account: readerOfTips,
pw: tipReader123) is needed to read the tips. You need to login to the mysql program and do:

    mysql> create database HPC_Tips;
    mysql> create user 'tipBuilder'@'%'           identified by 'test623';
    mysql> create user 'tipBuilder'@'localhost'   identified by 'test623';
    mysql> create user 'readerOfTips'@'%'         identified by 'tipReader123';
    mysql> create user 'readerOfTips'@'localhost' identified by 'tipReader123';
    mysql> grant all privileges ON HPC_Tips.* TO 'tipBuilder'@'localhost';
    mysql> grant all privileges ON HPC_Tips.* TO 'tipBuilder'@'%';
    mysql> grant select ON HPC_Tips.* TO 'readerOfTips'@'localhost';
    mysql> grant select ON HPC_Tips.* TO 'readerOfTips'@'%';
    mysql> flush privileges;

Obviously, you can use any names and password you like, you just need to be consistent throughout this process.


2) Create database config file:
 
The following program will create a database configuration file used by the **load_tips.py** program

    $ ./conf_create.py

    Database host: alice
    Database user: tipBuilder
    Database pass: test623
    Database name: HPC_Tips
    
If you want to just load the database from the server then replace "alice" with "localhost".  This program will use the database
name you used to create a file.  In this case it will use "HPC_Tips_db.conf".

3) Edit tips

The tips text files are broken into two files at are used at TACC.  There is a general.tips file which contains general tips that
should be useful at all sites.  The second file is the tacc.tips file.  This contains tips that may need to be modified for other
sites.  This file could be moved out of this directory and edited at a later date.

The following rules are used with the tips:

  a) All tips are between lines "##---" as shown below:

     ##----
     It usually takes 30 minutes or more for new passwords to propagate across TACC systems.
     ##----

  b) Long lines are filled to fit the line width on the terminal.
  c) Newlines are honored when formatting the tip for the terminal.
  d) leading blanks are honored when formatting the tip for the terminal.

4) load the database:

      ./load_tips.py HPC_Tips_db.conf

Note that the **load_tips.py** program uses the first argument given as the name of the config file.  If no argument is given
it tries either "HPC_Tips_db.conf" or "HPCTips_db.conf"


5) Check that you can access the database from the login nodes:

     % mysql -u readerOfTips -h alice -p HPC_Tips 
     Enter password: 
     mysql> select * from tips limit 1;

If this doesn't work then check to see that you can read the tips on the server and then check the firewall rules to
allow the mysql ports access the "alice" server.

6) Build the **showTips** program:

      % ./configure --prefix=/path/to/HPCTips --with-host=alice --with-reader=readerOfTips --with-pass=tipReader123 --with-db=HPC_Tips --with-module=HPC_Tips
      % make install

7) Test **showTips** program to see if works:

      % /path/to/HPCTips/bin/showTips -n 1
      % /path/to/HPCTips/bin/showTips -w
      % /path/to/HPCTips/bin/showTips -a

8) Install **showTips** program and the HPC_Tips modulefile on your system

At TACC, we install the **showTips** program in /opt/apps/HPC_Tips/<version>/bin/showTip. Then in the /opt/apps/HPC_Tips directory
we make a symbolic link between the version and HPC_Tips. This way we have a fixed location for the **showTips** binary. This makes
the next step easier.

A copy of our module file can be found in modules/HPC_Tips.lua


9) Add the files init/z99_HPC_Tips.sh and init/z99_HPC_Tips.csh to your /etc/profile.d directory for all login nodes

You will need to specify the location of the **showTips** binary in each file.
