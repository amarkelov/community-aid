--
-- Table structure for table main_menu
--

DROP TABLE  main_menu CASCADE;
CREATE TABLE main_menu (
  mid serial NOT NULL,
  menu_name varchar(256) NOT NULL default '',
  menu_desc varchar(1024) NOT NULL default '',
  menu_page varchar(128) NOT NULL default 'community-aid.php',
  isAdmin boolean default 'f',
  PRIMARY KEY  (mid),
  UNIQUE (menu_name)
);

--
-- Table structure for table l1_class
--

DROP TABLE  l1_class CASCADE;
CREATE TABLE l1_class (
  l1id bigserial NOT NULL,
  l1name varchar(64) NOT NULL default '',
  PRIMARY KEY  (l1id),
  UNIQUE (l1name)
);

--
-- Table structure for table l2_class
--

DROP TABLE  l2_class CASCADE;
CREATE TABLE l2_class (
  l2id bigserial NOT NULL,
  l2name varchar(64) NOT NULL default '',
  l1id bigint NOT NULL,
  PRIMARY KEY (l2id),
  FOREIGN KEY (l1id) REFERENCES l1_class (l1id) ON DELETE CASCADE ON UPDATE CASCADE,
  UNIQUE (l2name)
);

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
-- Table structure for table call_sclass
--

DROP TABLE  call_sclass CASCADE;
CREATE TABLE call_sclass (
  mclass_id bigserial,
  sclass_id bigserial NOT NULL,
  sclass_name varchar(64) NOT NULL default '',
  PRIMARY KEY  (mclass_id,sclass_name),
  UNIQUE (sclass_id,sclass_name),
  UNIQUE (sclass_id),
  FOREIGN KEY (mclass_id) REFERENCES call_mclass (mclass_id) ON DELETE CASCADE ON UPDATE CASCADE
);

--
-- Table structure for table operators
--

DROP TABLE  operators CASCADE;
CREATE TABLE operators (
  operatorid bigserial,
  loginname varchar(255) NOT NULL,
  fullname varchar(64) NOT NULL,
  saltypwd text NOT NULL,
  isadmin boolean default 'f',
  deleted boolean default 'f',
  added timestamp NOT NULL default CURRENT_TIMESTAMP,
  addedby bigint NOT NULL,
  modified timestamp NOT NULL default CURRENT_TIMESTAMP,
  modifiedby bigint NOT NULL,
  PRIMARY KEY  (operatorid),
  UNIQUE (loginname)
);

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
-- Table structure for table hseareas
--

DROP TABLE  hseareas CASCADE;
CREATE TABLE hseareas (
  hseareaid bigserial,
  hsearea_name varchar(128) NOT NULL,
  comments varchar(256) default NULL,
  PRIMARY KEY  (hseareaid),
  UNIQUE (hsearea_name)
);

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
  address varchar(512) NOT NULL,
  area varchar(50) NOT NULL,
  phone1 varchar(32) NOT NULL,
  phone2 varchar(32) NOT NULL,
  housetype varchar(20) default NULL,
  dob date NOT NULL,
  alone boolean default 'f',
  contact1name varchar(64) NOT NULL,
  contact1relationship varchar(128) default NULL,
  contact1address varchar(512) default NULL,
  contact1phone varchar(32) NOT NULL,
  contact2name varchar(64) default NULL,
  contact2relationship varchar(128) default NULL,
  contact2address varchar(512) default NULL,
  contact2phone varchar(32) default NULL,
  gpname varchar(64) default NULL,
  referrer varchar(30) default NULL,
  timeslot time NOT NULL,
  alerts varchar(2048) default NULL,
  medical_notes varchar(2048) default NULL,
  modified timestamp NOT NULL default CURRENT_TIMESTAMP,
  active boolean default 't',
  addedby bigint NOT NULL,
  modifiedby bigint NOT NULL,
  changenote varchar(255) default NULL,
  districtid bigint NOT NULL,
  hseareaid bigint default 0,
  groupid bigint default 0,	
  PRIMARY KEY  (clientid),
  UNIQUE (firstname,lastname,address,dob),
  FOREIGN KEY (addedby) REFERENCES operators (operatorid) ON UPDATE CASCADE,
  FOREIGN KEY (modifiedby) REFERENCES operators (operatorid) ON UPDATE CASCADE,
  FOREIGN KEY (districtid) REFERENCES districts (districtid) ON UPDATE CASCADE,
  FOREIGN KEY (hseareaid) REFERENCES hseareas (hseareaid) ON UPDATE CASCADE,
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
--  class int default NULL, -- no longer used. New deployments should not use it.
  operatorid bigint NOT NULL,
  call_finished boolean default 'f',
  PRIMARY KEY  (callid),
  FOREIGN KEY (clientid) REFERENCES clients (clientid) ON DELETE CASCADE ON UPDATE CASCADE,
--  FOREIGN KEY (class) REFERENCES call_sclass (sclass_id) ON UPDATE CASCADE, -- no longer used. New deployments should not use it.
  FOREIGN KEY (operatorid) REFERENCES operators (operatorid) ON UPDATE CASCADE
);

--
-- Table structure for table call_l1_class
--

DROP TABLE  call_l1_class CASCADE;
CREATE TABLE call_l1_class (
  callid bigint NOT NULL,
  l1id bigint NOT NULL,
  FOREIGN KEY (callid) REFERENCES calls (callid) ON DELETE CASCADE ON UPDATE CASCADE,
  FOREIGN KEY (l1id) REFERENCES l1_class (l1id) ON DELETE CASCADE ON UPDATE CASCADE
);

--
-- Table structure for table call_l2_class
--

DROP TABLE  call_l2_class CASCADE;
CREATE TABLE call_l2_class (
  callid bigint NOT NULL,
  l2id bigint NOT NULL,
  FOREIGN KEY (callid) REFERENCES calls (callid) ON DELETE CASCADE ON UPDATE CASCADE,
  FOREIGN KEY (l2id) REFERENCES l2_class (l2id) ON DELETE CASCADE ON UPDATE CASCADE
);

--
-- Table structure for table days
--

DROP TABLE  days CASCADE;
CREATE TABLE days (
  clientid bigint default NULL,
  dow int NOT NULL,
  FOREIGN KEY (clientid) REFERENCES clients (clientid) ON DELETE CASCADE ON UPDATE CASCADE,
  UNIQUE (clientid, dow)
);

--
-- Table structure for table group2operator 
--

DROP TABLE  group2operator CASCADE;
CREATE TABLE group2operator (
  groupid bigint NOT NULL,
  operatorid bigint NOT NULL,
  FOREIGN KEY (groupid) REFERENCES groups (groupid) ON UPDATE CASCADE,
  FOREIGN KEY (operatorid) REFERENCES operators (operatorid) ON DELETE CASCADE ON UPDATE CASCADE
);

--
-- View client2operator
--

CREATE OR REPLACE VIEW client2operator ( clientid, operatorid) AS 
SELECT	clientid,group2operator.operatorid FROM clients LEFT JOIN group2operator 
	ON clients.groupid=group2operator.groupid WHERE clients.groupid NOT IN (0,1);
	

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

--
-- Table structure for table client_nextcalltime
--

DROP TABLE  client_nextcalltime CASCADE;
CREATE TABLE client_nextcalltime (
  clientid bigint NOT NULL,
  nextcalltime timestamp NOT NULL,
  FOREIGN KEY (clientid) REFERENCES clients (clientid) ON DELETE CASCADE ON UPDATE CASCADE,
  UNIQUE (clientid)
);

-- CREATE INDEX calls_class_idx ON calls (class); -- no longer used. New deployments should not use it.
CREATE INDEX calls_clientid_idx ON calls (clientid);
CREATE INDEX calls_time_idx ON calls (time);
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
) RETURNS BIGINT AS $$ 
DECLARE
	inserted_clientid BIGINT;
BEGIN 
	/*
	 * Values casted to varchar(x) type explicitely to make sure that longer strings
	 * get truncated. Make sure the sizes correspond to the field size at all times!
	 * 
	 * trim function applied to firstname, lastname and address fields, so UNIQUE constrained
	 * in clients table works as expected. (Ref. the case when a client was put on the system
	 * twice bypassing the constrained, because a space was added to the end of address string)
	 *  
	 */
	INSERT INTO clients (trim(both from firstname),
						 trim(both from lastname),title,gender,
						 trim(both from address),area,districtid,
						phone1,phone2,housetype,dob,alone, medical_notes,
						contact1name,contact1relationship,contact1address,contact1phone,
						contact2name,contact2relationship, contact2address,contact2phone,
						gpname,referrer,alerts,timeslot,addedby,modifiedby, groupid)
	VALUES ( $1::varchar(64),  $2::varchar(64),  $3,  $4,  $5::varchar(512),  $6::varchar(50),  $7,
			 $8::varchar(32),  $9::varchar(32), $10, $11, $12, $13::varchar(2048), 
			 $14::varchar(64), $15::varchar(128), $16::varchar(512), $17::varchar(32), 
			 $18::varchar(64), $19::varchar(128), $20::varchar(512), $21::varchar(32),
			$22, $23::varchar(32), $24::varchar(2048), $25, $26, $26, $27);
	SELECT currval('clients_clientid_seq') INTO inserted_clientid;
	
	RETURN inserted_clientid;

END
$$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION client_timeslot_trigger() RETURNS TRIGGER AS $$ 
BEGIN 
	INSERT INTO client_timeslot_call (clientid) VALUES (NEW.clientid); 
	RETURN NULL;
END
$$ LANGUAGE plpgsql;
CREATE TRIGGER client_insert AFTER INSERT ON clients FOR EACH ROW EXECUTE PROCEDURE client_timeslot_trigger();

/*
 * The function records new calls and gets call's classification recorded.
 * Please, make sure you do it inside transaction!
 */
CREATE OR REPLACE FUNCTION record_call (clid BIGINT, call_chat TEXT, nxtcalltime TIMESTAMP, cfinished BOOLEAN, opid BIGINT) RETURNS BIGINT AS $$
DECLARE
	inserted_callid BIGINT;
BEGIN
	INSERT INTO calls (clientid, time, chat, call_finished, operatorid) VALUES ( clid, NOW(), call_chat, cfinished, opid);
	SELECT currval('calls_callid_seq') INTO inserted_callid;
	
	IF cfinished = 't' THEN
		UPDATE calls SET call_finished='t' WHERE clientid=clid;
		DELETE FROM client_nextcalltime WHERE clientid=clid;
	ELSE
		DELETE FROM client_nextcalltime WHERE clientid=clid;
		INSERT INTO client_nextcalltime (clientid,nextcalltime) VALUES (clid, nxtcalltime);
	END IF;
	
	RETURN inserted_callid;
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION operator_delete_before_trigger() RETURNS TRIGGER AS $$ 
BEGIN 
	/*
	 * In reality we are not going to delete the operator, only set him 'deleted'. 
	 * RETURN NULL secures that the BEFORE trigger prevents deletion but we NEVER 
	 * set deleted='t' for admin. 
	 * Also, the loginname of 'deleted' operator gets changed to loginname_date_time_of_deletion
	 * to be able to reuse the login name in the future.
	 */
	
	IF OLD.operatorid != 1 THEN
		UPDATE operators SET deleted='t',loginname=OLD.loginname || '_' || to_char(NOW(),'DDMMYYYY-HH24:MI:SS') 
			WHERE operatorid=OLD.operatorid;
	END IF;

	/* you don't really want to see 'deleted' operators in the assigned list */
	DELETE FROM group2operator WHERE operatorid=OLD.operatorid;

	RETURN NULL;
END
$$ LANGUAGE plpgsql;
CREATE TRIGGER operator_delete before DELETE ON operators FOR EACH ROW EXECUTE PROCEDURE operator_delete_before_trigger();

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

	RETURN NEW;
END
$$ LANGUAGE plpgsql;
CREATE TRIGGER operator_update AFTER UPDATE ON operators FOR EACH ROW EXECUTE PROCEDURE operator_update_after_trigger();

CREATE OR REPLACE VIEW clients_anonymous AS 
	SELECT cl.clientid,extract(year FROM age(dob)) AS age,gender,timeslot,alone,medical_notes,alerts,housetype,active,
	d.district_name,area,g.group_name from clients cl 
	LEFT OUTER JOIN districts d ON cl.districtid=d.districtid
	LEFT OUTER JOIN groups g ON cl.groupid=g.groupid
	ORDER BY cl.clientid;

CREATE OR REPLACE VIEW operators_report AS 
	SELECT operatorid, fullname, deleted FROM operators
	WHERE deleted=false
	ORDER BY operatorid;
	
CREATE OR REPLACE VIEW calls_report AS
	SELECT callid,clientid,time,chat,call_finished,c.operatorid,o.fullname AS operator_name FROM calls AS c 
	LEFT OUTER JOIN operators o ON c.operatorid=o.operatorid ORDER BY callid;

CREATE OR REPLACE VIEW cl1 AS SELECT callid,cl1.l1id,l1name FROM calls_report 
	LEFT OUTER JOIN call_l1_class cl1 USING (callid) 
	LEFT OUTER JOIN l1_class l1 ON cl1.l1id=l1.l1id;

CREATE OR REPLACE VIEW cl2 AS SELECT callid,cl2.l2id,l2name,l1id FROM calls_report 
	LEFT OUTER JOIN call_l2_class cl2 USING (callid) 
	LEFT OUTER JOIN l2_class l2 ON cl2.l2id=l2.l2id;
