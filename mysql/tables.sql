USE gmpDb;

--
-- Table structure for table `call_mclass`
--

DROP TABLE IF EXISTS `call_mclass`;
CREATE TABLE `call_mclass` (
  `mclass_id` int(11) NOT NULL auto_increment,
  `mclass_name` varchar(64) NOT NULL default '',
  PRIMARY KEY  (`mclass_id`),
  UNIQUE KEY `mclass_name` (`mclass_name`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Table structure for table `call_sclass`
--

DROP TABLE IF EXISTS `call_sclass`;
CREATE TABLE `call_sclass` (
  `mclass_id` int(11) NOT NULL default '0',
  `sclass_id` int(11) NOT NULL auto_increment,
  `sclass_name` varchar(64) NOT NULL default '',
  `sclass_sname` varchar(16) NOT NULL default '',
  PRIMARY KEY  (`mclass_id`,`sclass_name`),
  UNIQUE KEY `sclass_id` (`sclass_id`,`sclass_name`),
  UNIQUE KEY `sclass_sname` (`sclass_sname`),
  CONSTRAINT `call_sclass_ibfk_1` FOREIGN KEY (`mclass_id`) REFERENCES `call_mclass` (`mclass_id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Table structure for table `calls`
--

DROP TABLE IF EXISTS `calls`;
CREATE TABLE `calls` (
  `callid` int(11) NOT NULL auto_increment,
  `clientid` smallint(6) default NULL,
  `time` timestamp NOT NULL default CURRENT_TIMESTAMP on update CURRENT_TIMESTAMP,
  `chat` text,
  `class` int(11) default NULL,
  PRIMARY KEY  (`callid`),
  KEY `class` (`class`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 PACK_KEYS=1;

--
-- Table structure for table `clients`
--

DROP TABLE IF EXISTS `clients`;
CREATE TABLE `clients` (
  `clientid` smallint(4) NOT NULL auto_increment,
  `firstname` varchar(30) default NULL,
  `lastname` varchar(30) default NULL,
  `initials` varchar(5) default NULL,
  `title` varchar(20) default NULL,
  `houseno` varchar(15) default NULL,
  `postcode` varchar(8) default NULL,
  `street` varchar(30) default NULL,
  `town` varchar(50) default NULL,
  `phone1` varchar(15) default NULL,
  `phone2` varchar(15) default NULL,
  `phone3` varchar(15) default NULL,
  `description1` varchar(50) default NULL,
  `description2` varchar(50) default NULL,
  `housetype` varchar(20) default NULL,
  `dob` date default NULL,
  `startdate` date default NULL,
  `leavedate` date default NULL,
  `alone` tinyint(1) default '0',
  `contact1name` varchar(30) default NULL,
  `contact1relationship` varchar(20) default NULL,
  `contact1address` varchar(50) default NULL,
  `contact1phone1` varchar(15) default NULL,
  `contact1phone2` varchar(15) default NULL,
  `contact2name` varchar(30) default NULL,
  `contact2relationship` varchar(20) default NULL,
  `contact2address` varchar(50) default NULL,
  `contact2phone1` varchar(15) default NULL,
  `contact2phone2` varchar(15) default NULL,
  `gpname` varchar(30) default NULL,
  `hvname` varchar(30) default NULL,
  `housing` varchar(30) default NULL,
  `swname` varchar(30) default NULL,
  `referrer` varchar(30) default NULL,
  `timeslot` time default NULL,
  `list` varchar(30) default NULL,
  `ailments` varchar(255) default NULL,
  `note` varchar(255) default NULL,
  `timenote` varchar(255) default NULL,
  `modified` timestamp NOT NULL default CURRENT_TIMESTAMP on update CURRENT_TIMESTAMP,
  `done` varchar(5) default NULL,
  PRIMARY KEY  (`clientid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 PACK_KEYS=1;
