DROP DATABASE IF EXISTS community_aid_db;
CREATE DATABASE community_aid_db;
USE community_aid_db;

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
-- Dumping data for table `call_mclass`
--

LOCK TABLES `call_mclass` WRITE;
/*!40000 ALTER TABLE `call_mclass` DISABLE KEYS */;
INSERT INTO `call_mclass` VALUES (1,'Antisocial behaviour'),(4,'Community Services'),(5,'Financial Resources'),(2,'Health Service'),(3,'Housing'),(6,'Mobility'),(7,'Others');
/*!40000 ALTER TABLE `call_mclass` ENABLE KEYS */;
UNLOCK TABLES;

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
-- Dumping data for table `call_sclass`
--

LOCK TABLES `call_sclass` WRITE;
/*!40000 ALTER TABLE `call_sclass` DISABLE KEYS */;
INSERT INTO `call_sclass` VALUES (1,1,'Bullying/Intimidation','AB:BI'),(1,2,'Excess Noise','AB:EN'),(1,4,'Others','AB:OR'),(1,3,'Physical Abuse','AB:PA'),(2,8,'Chiropodist','HS:CT'),(2,12,'Condition of Health','HS:CH'),(2,10,'Dental Care','HS:DC'),(2,5,'Dr/Nurse','HS:DN'),(2,6,'Health Centre','HS:HC'),(2,7,'Hospital','HS:HL'),(2,9,'Optician','HS:ON'),(2,11,'Others','HS:OR'),(3,13,'General Maintenance','HG:GM'),(3,14,'Housing Aid','HG:HA'),(3,16,'Others','HG:OR'),(3,15,'Water','HG:WR'),(4,19,'Carers Association','CS:CA'),(4,17,'Home Help','CS:HH'),(4,18,'Meals on Wheels','CS:MW'),(4,21,'Others','CS:OR'),(4,20,'Senior Clubs','CS:SC'),(5,23,'Grants','FR:GT'),(5,24,'Money Difficulties','FR:MD'),(5,25,'Other','FR:OR'),(5,22,'Pension','FR:PN'),(6,27,'Other','MY:OR'),(6,26,'Public Transport','MY:PT'),(7,28,'Details','OR:DT');
/*!40000 ALTER TABLE `call_sclass` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `operators`
--

DROP TABLE IF EXISTS `operators`;
CREATE TABLE `operators` (
  `operatorid` bigint NOT NULL auto_increment,
  `loginname` varchar(25) NOT NULL,
  `fullname` tinytext NOT NULL,
  `saltypwd` tinyblob NOT NULL,
  `isAdmin` tinyint(1) default '0',
  `isSnr` tinyint(1) default '0',  
  `added` timestamp NOT NULL default CURRENT_TIMESTAMP,
  `addedby` bigint NOT NULL,
  `modified` timestamp NOT NULL,
  `modifiedby` bigint NOT NULL,
  PRIMARY KEY  (`operatorid`),
  UNIQUE KEY `loginname` (`loginname`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `operators`
--

LOCK TABLES `operators` WRITE;
/*!40000 ALTER TABLE `operators` DISABLE KEYS */;
INSERT INTO `operators` (operatorid, loginname, fullname, saltypwd, isAdmin, isSnr, added, addedby, modified, modifiedby) VALUES (1,'admin','Administrator','9bc7aa55f08fdad935c3f8362d3f48bcf70eb280',1,1,NOW(),0,NOW(),0);
/*!40000 ALTER TABLE `operators` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `districts`
--

DROP TABLE IF EXISTS `districts`;
CREATE TABLE `districts` (
  `districtid` bigint NOT NULL,
  `district_name` varchar(128) NOT NULL,
  `comments` varchar(256) default NULL,
  PRIMARY KEY  (`districtid`),
  UNIQUE KEY `district_name` (`district_name`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `districts`
--

LOCK TABLES `districts` WRITE;
/*!40000 ALTER TABLE `districts` DISABLE KEYS */;
INSERT INTO `districts` VALUES (1,'Blachardstown',NULL),(2,'Cabra',NULL),(3,'Stoneybatter',NULL),(4,'Inner City',NULL),(5,'Marino/Fairview',NULL),(6,'Clontarf',NULL),(7,'Finglas/Glasnevin',NULL),(8,'Darndale/Coolock',NULL),(9,'Baldoyle',NULL),(10,'Howth/Sutton',NULL),(11,'Portmarnock',NULL),(12,'Malahide',NULL),(13,'Skerries/Lusk/Rush',NULL),(14,'Donabate/Portrane',NULL),(15,'Swords',NULL),(16,'Santry',NULL),(17,'Ballymun',NULL),(18,'Artaine',NULL);
/*!40000 ALTER TABLE `districts` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `clients`
--

DROP TABLE IF EXISTS `clients`;
CREATE TABLE `clients` (
  `clientid` bigint NOT NULL auto_increment,
  `firstname` varchar(64) NOT NULL,
  `lastname` varchar(64) NOT NULL,
  `initials` varchar(5) default NULL,
  `title` varchar(20) default NULL,
  `gender` varchar(8) NOT NULL default 'female',
  `address` varchar(128) NOT NULL,
  `area` varchar(50) NOT NULL,
  `phone1` varchar(32) NOT NULL,
  `phone2` varchar(32) default NULL,
  `referrer_other` varchar(50) default NULL,
  `housetype` varchar(20) default NULL,
  `dob` date NOT NULL,
  `alone` tinyint(1) default '0',
  `contact1name` varchar(64) NOT NULL,
  `contact1relationship` varchar(20) default NULL,
  `contact1address` varchar(50) default NULL,
  `contact1phone1` varchar(32) NOT NULL,
  `contact1phone2` varchar(15) default NULL,
  `contact2name` varchar(30) default NULL,
  `contact2relationship` varchar(20) default NULL,
  `contact2address` varchar(50) default NULL,
  `contact2phone1` varchar(15) default NULL,
  `contact2phone2` varchar(15) default NULL,
  `gpname` varchar(64) default NULL,
  `referrer` varchar(30) default NULL,
  `timeslot` time NOT NULL,
  `ailments` varchar(255) default NULL,
  `note` varchar(255) default NULL,
  `modified` timestamp NOT NULL default CURRENT_TIMESTAMP on update CURRENT_TIMESTAMP,
  `active` tinyint(1) default '1',
  `addedby` bigint NOT NULL,
  `modifiedby` bigint NOT NULL,
  `changenote` varchar(255) default NULL,
  `districtid` bigint NOT NULL,
  PRIMARY KEY  (`clientid`),
  UNIQUE KEY `firstname` (`firstname`,`lastname`,`address`,`dob`),
  KEY `addedby` (`addedby`),
  KEY `modifiedby` (`modifiedby`),
  KEY `districtid` (`districtid`),
  CONSTRAINT `clients_ibfk_3` FOREIGN KEY (`districtid`) REFERENCES `districts` (`districtid`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `clients_ibfk_1` FOREIGN KEY (`addedby`) REFERENCES `operators` (`operatorid`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `clients_ibfk_2` FOREIGN KEY (`modifiedby`) REFERENCES `operators` (`operatorid`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Table structure for table `calls`
--

DROP TABLE IF EXISTS `calls`;
CREATE TABLE `calls` (
  `callid` bigint NOT NULL auto_increment,
  `clientid` bigint default NULL,
  `time` timestamp NOT NULL default CURRENT_TIMESTAMP on update CURRENT_TIMESTAMP,
  `chat` text,
  `class` int(11) default NULL,
  `operatorid` bigint NOT NULL,
  `nextcalltime` timestamp NOT NULL,
  `call_finished` tinyint(1) default '0',
  PRIMARY KEY  (`callid`),
  KEY `time_idx` (`time`),
  KEY `class_idx` (`class`),
  KEY `clientid_idx` (`clientid`),
  KEY `operatorid_fk` (`operatorid`),
  CONSTRAINT `calls_ibfk_1` FOREIGN KEY (`operatorid`) REFERENCES `operators` (`operatorid`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `calls_ibfk_2` FOREIGN KEY (`clientid`) REFERENCES `clients` (`clientid`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;


--
-- Table structure for table `client_timeslot_call`
--

DROP TABLE IF EXISTS `client_timeslot_call`;
CREATE TABLE `client_timeslot_call` (
  `clientid` bigint NOT NULL,
  `timeslot_done` tinyint(1) default '0',
  CONSTRAINT `client_timeslot_call_ibfk_1` FOREIGN KEY (`clientid`) REFERENCES `clients` (`clientid`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;


--
-- Triggers
--

CREATE TRIGGER client_timeslot_call_trgr AFTER INSERT ON clients FOR EACH ROW INSERT INTO client_timeslot_call (`clientid`) VALUES (NEW.clientid);

--
-- Table structure for table `client2operator`
--

DROP TABLE IF EXISTS `client2operator`;
CREATE TABLE `client2operator` (
  `clientid` bigint NOT NULL,
  `operatorid` bigint NOT NULL,
  KEY `clientid` (`clientid`),
  KEY `operatorid` (`operatorid`),
  CONSTRAINT `client2operator_ibfk_1` FOREIGN KEY (`clientid`) REFERENCES `clients` (`clientid`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `client2operator_ibfk_2` FOREIGN KEY (`operatorid`) REFERENCES `operators` (`operatorid`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

GRANT SELECT, INSERT, UPDATE, DELETE ON `community_aid_db`.`client2operator` TO 'caadmin'@'localhost';
GRANT SELECT, INSERT, UPDATE, DELETE ON `community_aid_db`.`call_mclass` TO 'caadmin'@'localhost';
GRANT SELECT, INSERT, UPDATE, DELETE ON `community_aid_db`.`operators` TO 'caadmin'@'localhost';
GRANT SELECT, INSERT, UPDATE, DELETE ON `community_aid_db`.`call_sclass` TO 'caadmin'@'localhost';
GRANT SELECT, INSERT, UPDATE ON `community_aid_db`.`clients` TO 'caadmin'@'localhost';
GRANT SELECT, INSERT, UPDATE ON `community_aid_db`.`client_timeslot_call` TO 'caadmin'@'localhost';
GRANT SELECT, INSERT, UPDATE ON `community_aid_db`.`calls` TO 'caadmin'@'localhost';
GRANT SELECT, INSERT, UPDATE, DELETE ON `community_aid_db`.`districts` TO 'caadmin'@'localhost';

GRANT SELECT ON `community_aid_db`.* TO 'caoperator'@'localhost'; 
GRANT INSERT, DELETE ON `community_aid_db`.`client2operator` TO 'caoperator'@'localhost';
GRANT SELECT, INSERT, UPDATE ON `community_aid_db`.`calls` TO 'caoperator'@'localhost'; 
GRANT SELECT, UPDATE ON `community_aid_db`.`client_timeslot_call` TO 'caoperator'@'localhost'; 
GRANT SELECT, UPDATE ON `community_aid_db`.`operators` TO 'caoperator'@'localhost'; 
GRANT SELECT ON `community_aid_db`.`districts` TO 'caoperator'@'localhost';

GRANT SELECT ON `community_aid_db`.* TO 'careport'@'localhost'; 

SET PASSWORD FOR 'caadmin'@'localhost' = PASSWORD('caadmin');
SET PASSWORD FOR 'caoperator'@'localhost' = PASSWORD('caoperator');
SET PASSWORD FOR 'careport'@'localhost' = PASSWORD('careport');
