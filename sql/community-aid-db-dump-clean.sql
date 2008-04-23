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

INSERT INTO call_mclass (mclass_name) VALUES ('Antisocial behaviour');
INSERT INTO call_mclass (mclass_name) VALUES ('Health Service');
INSERT INTO call_mclass (mclass_name) VALUES ('Housing');
INSERT INTO call_mclass (mclass_name) VALUES ('Community Services');
INSERT INTO call_mclass (mclass_name) VALUES ('Financial Resources');
INSERT INTO call_mclass (mclass_name) VALUES ('Mobility');
INSERT INTO call_mclass (mclass_name) VALUES ('Others');

--
-- Table structure for table call_sclass
--

DROP TABLE  call_sclass CASCADE;
CREATE TABLE call_sclass (
  mclass_id bigserial,
  sclass_id bigserial NOT NULL,
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

INSERT INTO call_sclass (mclass_id,sclass_name,sclass_sname) VALUES (1,'Bullying/Intimidation','AB:BI');
INSERT INTO call_sclass (mclass_id,sclass_name,sclass_sname) VALUES (1,'Excess Noise','AB:EN');
INSERT INTO call_sclass (mclass_id,sclass_name,sclass_sname) VALUES (1,'Others','AB:OR');
INSERT INTO call_sclass (mclass_id,sclass_name,sclass_sname) VALUES (1,'Physical Abuse','AB:PA');
INSERT INTO call_sclass (mclass_id,sclass_name,sclass_sname) VALUES (2,'Chiropodist','HS:CT');
INSERT INTO call_sclass (mclass_id,sclass_name,sclass_sname) VALUES (2,'Condition of Health','HS:CH');
INSERT INTO call_sclass (mclass_id,sclass_name,sclass_sname) VALUES (2,'Dental Care','HS:DC');
INSERT INTO call_sclass (mclass_id,sclass_name,sclass_sname) VALUES (2,'Dr/Nurse','HS:DN');
INSERT INTO call_sclass (mclass_id,sclass_name,sclass_sname) VALUES (2,'Health Centre','HS:HC');
INSERT INTO call_sclass (mclass_id,sclass_name,sclass_sname) VALUES (2,'Hospital','HS:HL');
INSERT INTO call_sclass (mclass_id,sclass_name,sclass_sname) VALUES (2,'Optician','HS:ON');
INSERT INTO call_sclass (mclass_id,sclass_name,sclass_sname) VALUES (2,'Others','HS:OR');
INSERT INTO call_sclass (mclass_id,sclass_name,sclass_sname) VALUES (3,'General Maintenance','HG:GM');
INSERT INTO call_sclass (mclass_id,sclass_name,sclass_sname) VALUES (3,'Housing Aid','HG:HA');
INSERT INTO call_sclass (mclass_id,sclass_name,sclass_sname) VALUES (3,'Others','HG:OR');
INSERT INTO call_sclass (mclass_id,sclass_name,sclass_sname) VALUES (3,'Water','HG:WR');
INSERT INTO call_sclass (mclass_id,sclass_name,sclass_sname) VALUES (4,'Carers Association','CS:CA');
INSERT INTO call_sclass (mclass_id,sclass_name,sclass_sname) VALUES (4,'Home Help','CS:HH');
INSERT INTO call_sclass (mclass_id,sclass_name,sclass_sname) VALUES (4,'Meals on Wheels','CS:MW');
INSERT INTO call_sclass (mclass_id,sclass_name,sclass_sname) VALUES (4,'Others','CS:OR');
INSERT INTO call_sclass (mclass_id,sclass_name,sclass_sname) VALUES (4,'Senior Clubs','CS:SC');
INSERT INTO call_sclass (mclass_id,sclass_name,sclass_sname) VALUES (5,'Grants','FR:GT');
INSERT INTO call_sclass (mclass_id,sclass_name,sclass_sname) VALUES (5,'Money Difficulties','FR:MD');
INSERT INTO call_sclass (mclass_id,sclass_name,sclass_sname) VALUES (5,'Other','FR:OR');
INSERT INTO call_sclass (mclass_id,sclass_name,sclass_sname) VALUES (5,'Pension','FR:PN');
INSERT INTO call_sclass (mclass_id,sclass_name,sclass_sname) VALUES (6,'Other','MY:OR');
INSERT INTO call_sclass (mclass_id,sclass_name,sclass_sname) VALUES (6,'Public Transport','MY:PT');
INSERT INTO call_sclass (mclass_id,sclass_name,sclass_sname) VALUES (7,'Details','OR:DT');

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
  deleted boolean default 'f',
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

INSERT INTO operators (loginname,fullname,saltypwd,isadmin,issnr, addedby,modifiedby) VALUES ('admin','Administrator',md5('adminsalt'),'t','t',1,1);

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

INSERT INTO districts VALUES (0,'Not defined or removed',NULL);

--
-- Table structure for table groups
--

DROP TABLE  groups CASCADE;
CREATE TABLE groups
(
  groupid bigserial NOT NULL,
  group_name character varying(128) NOT NULL,
  PRIMARY KEY (groupid)
);

--
-- INSERT default data for table groups
--

INSERT INTO groups VALUES (0, 'N/A');
INSERT INTO groups (group_name) VALUES ('Floating list');
INSERT INTO groups VALUES (0,'N/A');
INSERT INTO groups VALUES (1,'Floating group');

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
  groupid bigint default 0,	
  PRIMARY KEY  (clientid),
  UNIQUE (firstname,lastname,address,dob),
  FOREIGN KEY (addedby) REFERENCES operators (operatorid) ON UPDATE CASCADE,
  FOREIGN KEY (modifiedby) REFERENCES operators (operatorid) ON UPDATE CASCADE,
  FOREIGN KEY (districtid) REFERENCES districts (districtid) ON UPDATE CASCADE,
  FOREIGN KEY (groupid) REFERENCES groups (groupid) ON UPDATE CASCADE ON DELETE SET DEFAULT
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
  FOREIGN KEY (operatorid) REFERENCES operators (operatorid) ON UPDATE CASCADE
);

--
-- Table structure for table client2operator
--

DROP TABLE  client2operator CASCADE;
CREATE TABLE client2operator (
  clientid bigint NOT NULL,
  operatorid bigint NOT NULL,
  FOREIGN KEY (clientid) REFERENCES clients (clientid) ON DELETE CASCADE ON UPDATE CASCADE,
  FOREIGN KEY (operatorid) REFERENCES operators (operatorid) ON UPDATE CASCADE
);

--
-- Table structure for table client2group 
--

DROP TABLE  group2operator CASCADE;
CREATE TABLE group2operator (
  groupid bigint NOT NULL,
  operatorid bigint NOT NULL,
  FOREIGN KEY (groupid) REFERENCES groups (groupid) ON UPDATE CASCADE,
  FOREIGN KEY (operatorid) REFERENCES operators (operatorid) ON DELETE CASCADE ON UPDATE CASCADE,
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

CREATE OR REPLACE FUNCTION add_client(
	/*1 firstname*/ varchar,
	/*2 lastname*/ varchar,
	/*3 title*/ varchar,
	/*4 gender*/ varchar,
	/*5 address*/ varchar,
	/*6 area*/ varchar,
	/*7 districtid*/ bigint,
	/*8 phone1*/ varchar,
	/*9 phone2*/ varchar,
	/*10 housetype*/ varchar,
	/*11 dob*/ date,
	/*12 alone*/ boolean, 
	/*13 medical_notes*/ varchar,
	/*14 contact1name*/ varchar,
	/*15 contact1relationship*/ varchar,
	/*16 contact1address*/ varchar,
	/*17 contact1phone*/ varchar,
	/*18 contact2name*/ varchar,
	/*19 contact2relationship*/ varchar, 
	/*20 contact2address*/ varchar,
	/*21 contact2phone*/ varchar,
	/*22 gpname*/ varchar,
	/*23 referrer*/ varchar,
	/*24 alerts*/ varchar,
	/*25 timeslot*/ time,
	/*26 operatorid*/ bigint,
	/*27 groupid*/ bigint
) RETURNS void AS $$ 
DECLARE
	opid RECORD;
BEGIN 
INSERT INTO clients (firstname,lastname,title,gender,address,area,districtid,
					phone1,phone2,housetype,dob,alone, medical_notes,
					contact1name,contact1relationship,contact1address,contact1phone,
					contact2name,contact2relationship, contact2address,contact2phone,
					gpname,referrer,alerts,timeslot,addedby,modifiedby, groupid)
VALUES ( $1,  $2,  $3,  $4,  $5,  $6,  $7,
		 $8,  $9, $10, $11, $12, $13, $14, 
		$15, $16, $17, $18, $19, $20, $21,
		$22, $23, $24, $25, $26, $26, $27);

END
$$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION client_timeslot_trigger() RETURNS TRIGGER AS $$ 
BEGIN 
	INSERT INTO client_timeslot_call (clientid) VALUES (NEW.clientid); 
	RETURN NULL;
END
$$ LANGUAGE plpgsql;
CREATE TRIGGER client_insert AFTER INSERT ON clients FOR EACH ROW EXECUTE PROCEDURE client_timeslot_trigger();

CREATE OR REPLACE FUNCTION operator_delete_after_trigger() RETURNS TRIGGER AS $$ 
DECLARE
	record RECORD;
BEGIN 
	SELECT * INTO record FROM operators WHERE issnr='t';
	IF OLD.operatorid = record.operatorid THEN
		UPDATE operators SET issnr='f' WHERE operatorid=OLD.operatorid;
		UPDATE operators SET issnr='t' WHERE operatorid=1;
	END IF;

	UPDATE client2operator SET operatorid=record.operatorid WHERE operatorid=OLD.operatorid;
	
	/* in reality we are not going to delete the operator */
	/* only set him deleted. RETURN NULL secures that the */
	/* BEFORE trigger prevents deletion                   */
	/* but we NEVER set deleted='t' for admin			  */
	IF OLD.operatorid != 1 THEN
		UPDATE operators SET deleted='t' WHERE operatorid=OLD.operatorid;
	END IF;

	/* you don't really want to see 'deleted' operators in the assigned list */
	DELETE from group2operator WHERE operatorid=OLD.operatorid;

	RETURN NULL;
END
$$ LANGUAGE plpgsql;
CREATE TRIGGER operator_delete AFTER DELETE ON operators FOR EACH ROW EXECUTE PROCEDURE operator_delete_after_trigger();

CREATE OR REPLACE FUNCTION operator_update_after_trigger() RETURNS TRIGGER AS $$
BEGIN
	/* 
	 * we do not want admin account to be locked 
	 * that's why we do not set deleted='t' 
	 */
	IF OLD.operatorid = 1 THEN
		RETURN NULL;
	END IF;

	/*
	 * for the rest of the operators it's fine to set deleted='t'
	 */

	/* add the reinstated operator to the floating list */
	INSERT INTO group2operator VALUES ( 1, NEW.operatorid);

	RETURN NEW;
END
$$ LANGUAGE plpgsql;
CREATE TRIGGER operator_update AFTER UPDATE ON operators FOR EACH ROW EXECUTE PROCEDURE operator_update_after_trigger();

CREATE OR REPLACE FUNCTION operator_insert_trigger() RETURNS TRIGGER AS $$ 
DECLARE
	record RECORD;
BEGIN 
	SELECT * INTO record FROM operators WHERE issnr='t';
	IF NEW.issnr = 't' THEN
		UPDATE operators SET issnr='f' WHERE operatorid=record.operatorid;
		UPDATE client2operator SET operatorid=NEW.operatorid WHERE operatorid=record.operatorid;
	END IF;
	
	/* insert the newly added operator for access to the Floating group */
	INSERT INTO group2operator VALUES (1, NEW.operatorid);

	RETURN NULL;
END
$$ LANGUAGE plpgsql;
CREATE TRIGGER operator_insert AFTER INSERT ON operators FOR EACH ROW EXECUTE PROCEDURE operator_insert_trigger();

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
GRANT ALL ON groups_groupid_seq TO caadmin;

GRANT SELECT, INSERT, UPDATE, DELETE ON call_mclass TO caadmin;
GRANT SELECT, INSERT, UPDATE, DELETE ON call_sclass TO caadmin;
GRANT SELECT, INSERT, UPDATE ON calls TO caadmin;
GRANT SELECT, INSERT, UPDATE, DELETE ON client2operator TO caadmin;
GRANT SELECT, INSERT, UPDATE ON client_timeslot_call TO caadmin;
GRANT SELECT, INSERT, UPDATE ON clients TO caadmin;
GRANT SELECT, INSERT, UPDATE, DELETE ON districts TO caadmin;
GRANT SELECT, INSERT, UPDATE, DELETE ON operators TO caadmin;
GRANT SELECT, INSERT, UPDATE, DELETE ON groups TO caadmin;
GRANT SELECT, INSERT, UPDATE, DELETE ON client2group TO caadmin;

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
