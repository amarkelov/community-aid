-- MySQL dump 10.11
--
-- Host: localhost    Database: community_aid_db
-- ------------------------------------------------------
-- Server version	5.0.32-Debian_7etch1-log

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

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
-- Table structure for table `calls`
--

DROP TABLE IF EXISTS `calls`;
CREATE TABLE `calls` (
  `callid` int(11) NOT NULL auto_increment,
  `clientid` smallint(6) default NULL,
  `time` timestamp NOT NULL default CURRENT_TIMESTAMP on update CURRENT_TIMESTAMP,
  `chat` text,
  `class` int(11) default NULL,
  `operatorid` int(11) NOT NULL,
  `nextcalltime` timestamp NOT NULL default '0000-00-00 00:00:00',
  `call_finished` tinyint(1) default '0',
  PRIMARY KEY  (`callid`),
  KEY `time_idx` (`time`),
  KEY `class_idx` (`class`),
  KEY `clientid_idx` (`clientid`),
  KEY `operatorid_fk` (`operatorid`),
  CONSTRAINT `calls_ibfk_1` FOREIGN KEY (`operatorid`) REFERENCES `operators` (`operatorid`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1 PACK_KEYS=1;


--
-- Table structure for table `client2operator`
--

DROP TABLE IF EXISTS `client2operator`;
CREATE TABLE `client2operator` (
  `clientid` int(11) NOT NULL,
  `operatorid` int(11) NOT NULL,
  KEY `clientid` (`clientid`),
  KEY `operatorid` (`operatorid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;


--
-- Table structure for table `client_timeslot_call`
--

DROP TABLE IF EXISTS `client_timeslot_call`;
CREATE TABLE `client_timeslot_call` (
  `clientid` int(11) NOT NULL,
  `timeslot_done` tinyint(1) default '0',
  UNIQUE KEY `clientid` (`clientid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;


--
-- Table structure for table `clients`
--

DROP TABLE IF EXISTS `clients`;
CREATE TABLE `clients` (
  `clientid` smallint(4) NOT NULL auto_increment,
  `firstname` varchar(64) NOT NULL,
  `lastname` varchar(64) NOT NULL,
  `initials` varchar(5) default NULL,
  `title` varchar(20) default NULL,
  `houseno` varchar(32) NOT NULL,
  `street` varchar(64) NOT NULL,
  `town` varchar(50) default NULL,
  `phone1` varchar(32) NOT NULL,
  `phone2` varchar(32) NOT NULL,
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
  `gpname` varchar(64) NOT NULL,
  `housing` varchar(30) default NULL,
  `referrer` varchar(30) default NULL,
  `timeslot` time NOT NULL,
  `ailments` varchar(255) default NULL,
  `note` varchar(255) default NULL,
  `modified` timestamp NOT NULL default CURRENT_TIMESTAMP on update CURRENT_TIMESTAMP,
  `active` tinyint(1) default '1',
  `addedby` int(11) NOT NULL,
  `modifiedby` int(11) NOT NULL,
  `changenote` varchar(255) default NULL,
  PRIMARY KEY  (`clientid`),
  UNIQUE KEY `firstname` (`firstname`,`lastname`,`houseno`,`street`,`dob`),
  KEY `addedby` (`addedby`),
  KEY `modifiedby` (`modifiedby`),
  CONSTRAINT `clients_ibfk_1` FOREIGN KEY (`addedby`) REFERENCES `operators` (`operatorid`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `clients_ibfk_2` FOREIGN KEY (`modifiedby`) REFERENCES `operators` (`operatorid`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1 PACK_KEYS=1;


/*!50003 SET @OLD_SQL_MODE=@@SQL_MODE*/;
DELIMITER ;;
/*!50003 SET SESSION SQL_MODE="" */;;
/*!50003 CREATE */ /*!50017 DEFINER=`root`@`localhost` */ /*!50003 TRIGGER `ins_clientid_into_client_timeslot_call` AFTER INSERT ON `clients` FOR EACH ROW insert into client_timeslot_call (clientid) values (NEW.clientid) */;;

DELIMITER ;
/*!50003 SET SESSION SQL_MODE=@OLD_SQL_MODE */;

--
-- Table structure for table `operators`
--

DROP TABLE IF EXISTS `operators`;
CREATE TABLE `operators` (
  `operatorid` int(11) NOT NULL auto_increment,
  `loginname` varchar(25) NOT NULL,
  `fullname` tinytext NOT NULL,
  `saltypwd` tinyblob NOT NULL,
  `isAdmin` tinyint(1) default '0',
  `added` timestamp NOT NULL default CURRENT_TIMESTAMP,
  `addedby` int(11) NOT NULL,
  `modified` timestamp NOT NULL default '0000-00-00 00:00:00',
  `modifiedby` int(11) NOT NULL,
  PRIMARY KEY  (`operatorid`),
  UNIQUE KEY `loginname` (`loginname`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `operators`
--

LOCK TABLES `operators` WRITE;
/*!40000 ALTER TABLE `operators` DISABLE KEYS */;
INSERT INTO `operators` VALUES (1,'admin','Administrator','9bc7aa55f08fdad935c3f8362d3f48bcf70eb280',1,NOW(),0,'0000-00-00 00:00:00',0);
/*!40000 ALTER TABLE `operators` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

GRANT SELECT, INSERT, UPDATE, DELETE ON `community_aid_db`.`client2operator` TO 'gmadmin'@'localhost';
GRANT SELECT, INSERT, UPDATE, DELETE ON `community_aid_db`.`call_mclass` TO 'gmadmin'@'localhost';
GRANT SELECT, INSERT, UPDATE, DELETE ON `community_aid_db`.`operators` TO 'gmadmin'@'localhost';
GRANT SELECT, INSERT, UPDATE, DELETE ON `community_aid_db`.`call_sclass` TO 'gmadmin'@'localhost';
GRANT SELECT, INSERT, UPDATE ON `community_aid_db`.`clients` TO 'gmadmin'@'localhost';
GRANT SELECT, INSERT, UPDATE ON `community_aid_db`.`client_timeslot_call` TO 'gmadmin'@'localhost';
GRANT SELECT, INSERT, UPDATE ON `community_aid_db`.`calls` TO 'gmadmin'@'localhost';

GRANT SELECT ON `community_aid_db`.* TO 'gmoperator'@'localhost'; 
GRANT SELECT, INSERT, UPDATE ON `community_aid_db`.`calls` TO 'gmoperator'@'localhost'; 
GRANT SELECT, UPDATE ON `community_aid_db`.`client_timeslot_call` TO 'gmoperator'@'localhost'; 
GRANT SELECT, UPDATE ON `community_aid_db`.`operators` TO 'gmoperator'@'localhost'; 

SET PASSWORD FOR 'gmadmin'@'localhost' = PASSWORD('gmadmin');
SET PASSWORD FOR 'gmoperator'@'localhost' = PASSWORD('gmoperator');

-- Dump completed on 2007-07-19 20:23:52
