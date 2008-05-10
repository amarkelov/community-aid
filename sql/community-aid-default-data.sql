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
-- INSERT default data for table call_sclass
--

INSERT INTO call_sclass (mclass_id,sclass_name) VALUES (1,'Bullying/Intimidation');
INSERT INTO call_sclass (mclass_id,sclass_name) VALUES (1,'Excess Noise');
INSERT INTO call_sclass (mclass_id,sclass_name) VALUES (1,'Others');
INSERT INTO call_sclass (mclass_id,sclass_name) VALUES (1,'Physical Abuse');
INSERT INTO call_sclass (mclass_id,sclass_name) VALUES (2,'Chiropodist');
INSERT INTO call_sclass (mclass_id,sclass_name) VALUES (2,'Condition of Health');
INSERT INTO call_sclass (mclass_id,sclass_name) VALUES (2,'Dental Care');
INSERT INTO call_sclass (mclass_id,sclass_name) VALUES (2,'Dr/Nurse');
INSERT INTO call_sclass (mclass_id,sclass_name) VALUES (2,'Health Centre');
INSERT INTO call_sclass (mclass_id,sclass_name) VALUES (2,'Hospital');
INSERT INTO call_sclass (mclass_id,sclass_name) VALUES (2,'Optician');
INSERT INTO call_sclass (mclass_id,sclass_name) VALUES (2,'Others');
INSERT INTO call_sclass (mclass_id,sclass_name) VALUES (3,'General Maintenance');
INSERT INTO call_sclass (mclass_id,sclass_name) VALUES (3,'Housing Aid');
INSERT INTO call_sclass (mclass_id,sclass_name) VALUES (3,'Others');
INSERT INTO call_sclass (mclass_id,sclass_name) VALUES (3,'Water');
INSERT INTO call_sclass (mclass_id,sclass_name) VALUES (4,'Carers Association');
INSERT INTO call_sclass (mclass_id,sclass_name) VALUES (4,'Home Help');
INSERT INTO call_sclass (mclass_id,sclass_name) VALUES (4,'Meals on Wheels');
INSERT INTO call_sclass (mclass_id,sclass_name) VALUES (4,'Others');
INSERT INTO call_sclass (mclass_id,sclass_name) VALUES (4,'Senior Clubs');
INSERT INTO call_sclass (mclass_id,sclass_name) VALUES (5,'Grants');
INSERT INTO call_sclass (mclass_id,sclass_name) VALUES (5,'Money Difficulties');
INSERT INTO call_sclass (mclass_id,sclass_name) VALUES (5,'Other');
INSERT INTO call_sclass (mclass_id,sclass_name) VALUES (5,'Pension');
INSERT INTO call_sclass (mclass_id,sclass_name) VALUES (6,'Other');
INSERT INTO call_sclass (mclass_id,sclass_name) VALUES (6,'Public Transport');
INSERT INTO call_sclass (mclass_id,sclass_name) VALUES (7,'Details');

--
-- INSERT default data for table districts
--

INSERT INTO districts VALUES (0,'Not defined or removed',NULL);

--
-- INSERT default data for table groups
--

INSERT INTO groups VALUES (0, 'N/A');
INSERT INTO groups (group_name) VALUES ('Floating list');

