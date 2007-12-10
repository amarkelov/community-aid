--
-- Table structure for table call_mclass
--

DROP TABLE  call_mclass CASCADE;
CREATE TABLE call_mclass (
  mclass_id bigserial,
  mclass_name varchar(64) NOT NULL default '',
  PRIMARY KEY  (mclass_id),
  UNIQUE (mclass_name)
);

--
-- INSERT default data for table call_mclass
--

INSERT INTO call_mclass VALUES (1,'Antisocial behaviour');
INSERT INTO call_mclass VALUES (2,'Health Service');
INSERT INTO call_mclass VALUES (3,'Housing');
INSERT INTO call_mclass VALUES (4,'Community Services');
INSERT INTO call_mclass VALUES (5,'Financial Resources');
INSERT INTO call_mclass VALUES (6,'Mobility');
INSERT INTO call_mclass VALUES (7,'Others');

--
-- Table structure for table call_sclass
--

DROP TABLE  call_sclass CASCADE;
CREATE TABLE call_sclass (
  mclass_id bigserial,
  sclass_id bigint NOT NULL,
  sclass_name varchar(64) NOT NULL default '',
  sclass_sname varchar(16) NOT NULL default '',
  PRIMARY KEY  (mclass_id,sclass_name),
  UNIQUE (sclass_id,sclass_name),
  UNIQUE (sclass_sname),
  FOREIGN KEY (mclass_id) REFERENCES call_mclass (mclass_id) ON DELETE CASCADE ON UPDATE CASCADE
);

--
-- INSERT default data for table call_sclass
--

INSERT INTO call_sclass VALUES (1,1,'Bullying/Intimidation','AB:BI');
INSERT INTO call_sclass VALUES (1,2,'Excess Noise','AB:EN');
INSERT INTO call_sclass VALUES (1,4,'Others','AB:OR');
INSERT INTO call_sclass VALUES (1,3,'Physical Abuse','AB:PA');
INSERT INTO call_sclass VALUES (2,8,'Chiropodist','HS:CT');
INSERT INTO call_sclass VALUES (2,12,'Condition of Health','HS:CH');
INSERT INTO call_sclass VALUES (2,10,'Dental Care','HS:DC');
INSERT INTO call_sclass VALUES (2,5,'Dr/Nurse','HS:DN');
INSERT INTO call_sclass VALUES (2,6,'Health Centre','HS:HC');
INSERT INTO call_sclass VALUES (2,7,'Hospital','HS:HL');
INSERT INTO call_sclass VALUES (2,9,'Optician','HS:ON');
INSERT INTO call_sclass VALUES (2,11,'Others','HS:OR');
INSERT INTO call_sclass VALUES (3,13,'General Maintenance','HG:GM');
INSERT INTO call_sclass VALUES (3,14,'Housing Aid','HG:HA');
INSERT INTO call_sclass VALUES (3,16,'Others','HG:OR');
INSERT INTO call_sclass VALUES (3,15,'Water','HG:WR');
INSERT INTO call_sclass VALUES (4,19,'Carers Association','CS:CA');
INSERT INTO call_sclass VALUES (4,17,'Home Help','CS:HH');
INSERT INTO call_sclass VALUES (4,18,'Meals on Wheels','CS:MW');
INSERT INTO call_sclass VALUES (4,21,'Others','CS:OR');
INSERT INTO call_sclass VALUES (4,20,'Senior Clubs','CS:SC');
INSERT INTO call_sclass VALUES (5,23,'Grants','FR:GT');
INSERT INTO call_sclass VALUES (5,24,'Money Difficulties','FR:MD');
INSERT INTO call_sclass VALUES (5,25,'Other','FR:OR');
INSERT INTO call_sclass VALUES (5,22,'Pension','FR:PN');
INSERT INTO call_sclass VALUES (6,27,'Other','MY:OR');
INSERT INTO call_sclass VALUES (6,26,'Public Transport','MY:PT');
INSERT INTO call_sclass VALUES (7,28,'Details','OR:DT');

--
-- Table structure for table operators
--

DROP TABLE  operators CASCADE;
CREATE TABLE operators (
  operatorid bigserial,
  loginname varchar(25) NOT NULL,
  fullname varchar(64) NOT NULL,
  saltypwd text NOT NULL,
  isadmin boolean default 'f',
  issnr boolean default 'f',
  added timestamp NOT NULL default CURRENT_TIMESTAMP,
  addedby bigint NOT NULL,
  modified timestamp NOT NULL default CURRENT_TIMESTAMP,
  modifiedby bigint NOT NULL,
  PRIMARY KEY  (operatorid),
  UNIQUE (loginname)
);

--
-- INSERT default data for table operators
--

INSERT INTO operators VALUES (1,'admin','Administrator',md5('adminsalt'),'t','f',NOW(),0,NOW(),0);

--
-- Table structure for table districts
--

DROP TABLE  districts CASCADE;
CREATE TABLE districts (
  districtid bigserial,
  district_name varchar(128) NOT NULL,
  comments varchar(256) default NULL,
  PRIMARY KEY  (districtid),
  UNIQUE (district_name)
);

--
-- INSERT default data for table districts
--

INSERT INTO districts VALUES (1,'Blachardstown',NULL);
INSERT INTO districts VALUES (2,'Cabra',NULL);
INSERT INTO districts VALUES (3,'Stoneybatter',NULL);
INSERT INTO districts VALUES (4,'Inner City',NULL);
INSERT INTO districts VALUES (5,'Marino/Fairview',NULL);
INSERT INTO districts VALUES (6,'Clontarf',NULL);
INSERT INTO districts VALUES (7,'Glasnevin',NULL);
INSERT INTO districts VALUES (8,'Coolock',NULL);
INSERT INTO districts VALUES (9,'Baldoyle',NULL);
INSERT INTO districts VALUES (10,'Howth/Sutton',NULL);
INSERT INTO districts VALUES (11,'Portmarnock',NULL);
INSERT INTO districts VALUES (12,'Malahide',NULL);
INSERT INTO districts VALUES (13,'Skerries/Lusk/Rush',NULL);
INSERT INTO districts VALUES (14,'Donabate/Portrane',NULL);
INSERT INTO districts VALUES (15,'Swords',NULL);
INSERT INTO districts VALUES (16,'Santry',NULL);
INSERT INTO districts VALUES (17,'Ballymun',NULL);
INSERT INTO districts VALUES (18,'Artaine',NULL);

--
-- Table structure for table clients
--

DROP TABLE  clients CASCADE;
CREATE TABLE clients (
  clientid bigserial,
  firstname varchar(64) NOT NULL,
  lastname varchar(64) NOT NULL,
  initials varchar(5) default NULL,
  title varchar(20) default NULL,
  gender varchar(8) NOT NULL default 'female',
  address varchar(128) NOT NULL,
  area varchar(50) NOT NULL,
  phone1 varchar(32) NOT NULL,
  phone2 varchar(32) NOT NULL,
  housetype varchar(20) default NULL,
  dob date NOT NULL,
  alone boolean default 'f',
  contact1name varchar(64) NOT NULL,
  contact1relationship varchar(20) default NULL,
  contact1address varchar(50) default NULL,
  contact1phone varchar(32) NOT NULL,
  contact2name varchar(30) default NULL,
  contact2relationship varchar(20) default NULL,
  contact2address varchar(50) default NULL,
  contact2phone varchar(15) default NULL,
  gpname varchar(64) default NULL,
  referrer varchar(30) default NULL,
  timeslot time NOT NULL,
  alerts varchar(255) default NULL,
  medical_notes varchar(255) default NULL,
  modified timestamp NOT NULL default CURRENT_TIMESTAMP,
  active boolean default 't',
  addedby bigint NOT NULL,
  modifiedby bigint NOT NULL,
  changenote varchar(255) default NULL,
  districtid bigint NOT NULL,
  PRIMARY KEY  (clientid),
  UNIQUE (firstname,lastname,address,dob),
  FOREIGN KEY (addedby) REFERENCES operators (operatorid) ON DELETE CASCADE ON UPDATE CASCADE,
  FOREIGN KEY (modifiedby) REFERENCES operators (operatorid) ON DELETE CASCADE ON UPDATE CASCADE,
  FOREIGN KEY (districtid) REFERENCES districts (districtid) ON DELETE CASCADE ON UPDATE CASCADE
);

--
-- Table structure for table calls
--

DROP TABLE  calls CASCADE;
CREATE TABLE calls (
  callid bigserial,
  clientid bigint default NULL,
  time timestamp NOT NULL default CURRENT_TIMESTAMP,
  chat text,
  class int default NULL,
  operatorid bigint NOT NULL,
  nextcalltime timestamp NOT NULL,
  call_finished boolean default 'f',
  PRIMARY KEY  (callid),
  FOREIGN KEY (clientid) REFERENCES clients (clientid) ON DELETE CASCADE ON UPDATE CASCADE,
  FOREIGN KEY (operatorid) REFERENCES operators (operatorid) ON DELETE CASCADE ON UPDATE CASCADE
);


--
-- Table structure for table client2operator
--

DROP TABLE  client2operator CASCADE;
CREATE TABLE client2operator (
  clientid bigint NOT NULL,
  operatorid bigint NOT NULL,
  FOREIGN KEY (clientid) REFERENCES clients (clientid) ON DELETE CASCADE ON UPDATE CASCADE,
  FOREIGN KEY (operatorid) REFERENCES operators (operatorid) ON DELETE CASCADE ON UPDATE CASCADE
);

--
-- Table structure for table client_timeslot_call
--

DROP TABLE  client_timeslot_call CASCADE;
CREATE TABLE client_timeslot_call (
  clientid bigint NOT NULL,
  timeslot_done boolean default 'f',
  FOREIGN KEY (clientid) REFERENCES clients (clientid) ON DELETE CASCADE ON UPDATE CASCADE,
  UNIQUE (clientid)
);

CREATE INDEX calls_class_idx ON calls (class);
CREATE INDEX calls_clientid_idx ON calls (clientid);
CREATE INDEX calls_time_idx ON calls (time);
CREATE INDEX client2operator_clientid_idx ON client2operator (clientid);
CREATE INDEX client2operator_operatorid_idx ON client2operator (operatorid);
CREATE INDEX clients_addedby_idx ON clients (addedby);
CREATE INDEX clients_modifiedby_idx ON clients (modifiedby);
CREATE INDEX clients_districtid_idx ON clients (districtid);


--
-- CREATE FUNCTIONS AND TRIGGER PROCEDURES
--

CREATE OR REPLACE FUNCTION client_timeslot_trigger() RETURNS TRIGGER AS $$ 
BEGIN 
	INSERT INTO client_timeslot_call (clientid) VALUES (NEW.clientid); 
	RETURN NULL;
END
$$ LANGUAGE plpgsql;

CREATE TRIGGER client_insert AFTER INSERT ON clients FOR EACH ROW EXECUTE PROCEDURE client_timeslot_trigger();

--
-- CREATE USERs
--
DROP USER caadmin;
DROP USER caoperator;
DROP USER careport;

CREATE USER caadmin WITH ENCRYPTED PASSWORD 'caadmin';
CREATE USER caoperator WITH ENCRYPTED PASSWORD 'caoperator';
CREATE USER careport WITH ENCRYPTED PASSWORD 'careport';

--
-- GRANT rights to default admin user
--

GRANT ALL ON call_mclass_mclass_id_seq TO caadmin;
GRANT ALL ON call_sclass_mclass_id_seq TO caadmin;
GRANT ALL ON calls_callid_seq TO caadmin;
GRANT ALL ON clients_clientid_seq TO caadmin;
GRANT ALL ON districts_districtid_seq TO caadmin;
GRANT ALL ON operators_operatorid_seq TO caadmin;

GRANT SELECT, INSERT, UPDATE, DELETE ON call_mclass TO caadmin;
GRANT SELECT, INSERT, UPDATE, DELETE ON call_sclass TO caadmin;
GRANT SELECT, INSERT, UPDATE ON calls TO caadmin;
GRANT SELECT, INSERT, UPDATE, DELETE ON client2operator TO caadmin;
GRANT SELECT, INSERT, UPDATE ON client_timeslot_call TO caadmin;
GRANT SELECT, INSERT, UPDATE ON clients TO caadmin;
GRANT SELECT, INSERT, UPDATE, DELETE ON districts TO caadmin;
GRANT SELECT, INSERT, UPDATE, DELETE ON operators TO caadmin;

--
-- GRANT rights to default operator user
--
GRANT ALL ON calls_callid_seq TO caoperator;
GRANT SELECT ON call_mclass TO caoperator;
GRANT SELECT ON call_sclass TO caoperator;
GRANT SELECT, INSERT, UPDATE ON calls TO caoperator;
GRANT SELECT, INSERT, DELETE ON client2operator TO caoperator;
GRANT SELECT, INSERT, UPDATE ON client_timeslot_call TO caoperator;
GRANT SELECT ON clients TO caoperator;
GRANT SELECT ON districts TO caoperator;
GRANT SELECT, UPDATE ON operators TO caoperator;

--
-- Creating default user to fetch reports and giving rights
--

GRANT SELECT ON call_mclass TO careport;
GRANT SELECT ON call_sclass TO careport;
GRANT SELECT ON calls TO careport;
GRANT SELECT ON client2operator TO careport;
GRANT SELECT ON client_timeslot_call TO careport;
GRANT SELECT ON clients TO careport;
GRANT SELECT ON districts TO careport;
GRANT SELECT ON operators TO careport;
