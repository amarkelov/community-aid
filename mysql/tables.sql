USE gmpDb;

--
-- Table structure for table `calls`
--

CREATE TABLE calls (
  callid int(11) NOT NULL auto_increment,
  clientid smallint(6) default NULL,
  time timestamp(14) NOT NULL,
  chat text,
  class varchar(12) default NULL,
  PRIMARY KEY  (callid)
) TYPE=MyISAM PACK_KEYS=1;


--
-- Table structure for table `clients`
--

CREATE TABLE clients (
  clientid smallint(4) NOT NULL auto_increment,
  firstname varchar(30) default NULL,
  lastname varchar(30) default NULL,
  initials varchar(5) default NULL,
  title varchar(20) default NULL,
  houseno varchar(15) default NULL,
  postcode varchar(8) default NULL,
  street varchar(30) default NULL,
  town varchar(50) default NULL,
  phone1 varchar(15) default NULL,
  phone2 varchar(15) default NULL,
  phone3 varchar(15) default NULL,
  description1 varchar(50) default NULL,
  description2 varchar(50) default NULL,
  housetype varchar(20) default NULL,
  dob date default NULL,
  startdate date default NULL,
  leavedate date default NULL,
  alone varchar(5) default NULL,
  contact1name varchar(30) default NULL,
  contact1relationship varchar(20) default NULL,
  contact1address varchar(50) default NULL,
  contact1phone1 varchar(15) default NULL,
  contact1phone2 varchar(15) default NULL,
  contact2name varchar(30) default NULL,
  contact2relationship varchar(20) default NULL,
  contact2address varchar(50) default NULL,
  contact2phone1 varchar(15) default NULL,
  contact2phone2 varchar(15) default NULL,
  gpname varchar(30) default NULL,
  hvname varchar(30) default NULL,
  housing varchar(30) default NULL,
  swname varchar(30) default NULL,
  referrer varchar(30) default NULL,
  timeslot varchar(11) NOT NULL default '',
  list varchar(30) default NULL,
  ailments varchar(255) default NULL,
  note varchar(255) default NULL,
  timenote varchar(255) default NULL,
  modified timestamp(14) NOT NULL,
  done varchar(5) default NULL,
  PRIMARY KEY  (clientid)
) TYPE=MyISAM PACK_KEYS=1;
