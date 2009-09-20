--
-- INSERT default data for table operators
--

INSERT INTO operators (loginname,fullname,saltypwd,isadmin,addedby,modifiedby) VALUES ('admin','Administrator',md5('adminsalt'),'t',1,1);

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

--
-- INSERT default data for table l1_class
--

INSERT INTO l1_class (l1id, l1name) VALUES (0, 'No issues today');
INSERT INTO l1_class (l1name) VALUES ('Safety & Security');
INSERT INTO l1_class (l1name) VALUES ('Social engagement');
INSERT INTO l1_class (l1name) VALUES ('Nutrition');
INSERT INTO l1_class (l1name) VALUES ('Mood');
INSERT INTO l1_class (l1name) VALUES ('Physical and health status');
INSERT INTO l1_class (l1name) VALUES ('Mobility');
INSERT INTO l1_class (l1name) VALUES ('Falls/accidents');

--
-- INSERT default data for table l2_class
--

INSERT INTO l2_class (l1id,l2name) VALUES ((SELECT l1id FROM l1_class WHERE l1name='Safety & Security'), 'Housing conditions');
INSERT INTO l2_class (l1id,l2name) VALUES ((SELECT l1id FROM l1_class WHERE l1name='Safety & Security'), 'Housing conditions');
INSERT INTO l2_class (l1id,l2name) VALUES ((SELECT l1id FROM l1_class WHERE l1name='Safety & Security'), 'Neighbourhood');
INSERT INTO l2_class (l1id,l2name) VALUES ((SELECT l1id FROM l1_class WHERE l1name='Safety & Security'), 'Feelings of threat');
INSERT INTO l2_class (l1id,l2name) VALUES ((SELECT l1id FROM l1_class WHERE l1name='Safety & Security'), 'Abuse');
INSERT INTO l2_class (l1id,l2name) VALUES ((SELECT l1id FROM l1_class WHERE l1name='Safety & Security'), 'Financial security');
INSERT INTO l2_class (l1id,l2name) VALUES ((SELECT l1id FROM l1_class WHERE l1name='Safety & Security'), 'Emergency services');
INSERT INTO l2_class (l1id,l2name) VALUES ((SELECT l1id FROM l1_class WHERE l1name='Safety & Security'), 'Security check');

INSERT INTO l2_class (l1id,l2name) VALUES ((SELECT l1id FROM l1_class WHERE l1name='Social engagement'), 'Activities, events');
INSERT INTO l2_class (l1id,l2name) VALUES ((SELECT l1id FROM l1_class WHERE l1name='Social engagement'), 'Loneliness');
INSERT INTO l2_class (l1id,l2name) VALUES ((SELECT l1id FROM l1_class WHERE l1name='Social engagement'), 'Family, friends');
INSERT INTO l2_class (l1id,l2name) VALUES ((SELECT l1id FROM l1_class WHERE l1name='Social engagement'), 'Pets');
INSERT INTO l2_class (l1id,l2name) VALUES ((SELECT l1id FROM l1_class WHERE l1name='Social engagement'), 'Formal services');

INSERT INTO l2_class (l1id,l2name) VALUES ((SELECT l1id FROM l1_class WHERE l1name='Mood'), 'Confusion');
INSERT INTO l2_class (l1id,l2name) VALUES ((SELECT l1id FROM l1_class WHERE l1name='Mood'), 'Feeling down/depression');
INSERT INTO l2_class (l1id,l2name) VALUES ((SELECT l1id FROM l1_class WHERE l1name='Mood'), 'Anxious');
INSERT INTO l2_class (l1id,l2name) VALUES ((SELECT l1id FROM l1_class WHERE l1name='Mood'), 'Contented');
INSERT INTO l2_class (l1id,l2name) VALUES ((SELECT l1id FROM l1_class WHERE l1name='Mood'), 'Coping well');
INSERT INTO l2_class (l1id,l2name) VALUES ((SELECT l1id FROM l1_class WHERE l1name='Mood'), 'Sleep');

INSERT INTO l2_class (l1id,l2name) VALUES ((SELECT l1id FROM l1_class WHERE l1name='Mobility'), 'Around home');
INSERT INTO l2_class (l1id,l2name) VALUES ((SELECT l1id FROM l1_class WHERE l1name='Mobility'), 'Outside house');
INSERT INTO l2_class (l1id,l2name) VALUES ((SELECT l1id FROM l1_class WHERE l1name='Mobility'), 'Aids');
INSERT INTO l2_class (l1id,l2name) VALUES ((SELECT l1id FROM l1_class WHERE l1name='Mobility'), 'Transport');

INSERT INTO l2_class (l1id,l2name) VALUES ((SELECT l1id FROM l1_class WHERE l1name='Physical and health status'), 'Medical conditions');
INSERT INTO l2_class (l1id,l2name) VALUES ((SELECT l1id FROM l1_class WHERE l1name='Physical and health status'), 'Medications');
INSERT INTO l2_class (l1id,l2name) VALUES ((SELECT l1id FROM l1_class WHERE l1name='Physical and health status'), 'Other treatments');
INSERT INTO l2_class (l1id,l2name) VALUES ((SELECT l1id FROM l1_class WHERE l1name='Physical and health status'), 'Sight');
INSERT INTO l2_class (l1id,l2name) VALUES ((SELECT l1id FROM l1_class WHERE l1name='Physical and health status'), 'Hearing');
INSERT INTO l2_class (l1id,l2name) VALUES ((SELECT l1id FROM l1_class WHERE l1name='Physical and health status'), 'Hygiene and dressing');

INSERT INTO l2_class (l1id,l2name) VALUES ((SELECT l1id FROM l1_class WHERE l1name='Nutrition'), 'Shopping');
INSERT INTO l2_class (l1id,l2name) VALUES ((SELECT l1id FROM l1_class WHERE l1name='Nutrition'), 'Preparation');
INSERT INTO l2_class (l1id,l2name) VALUES ((SELECT l1id FROM l1_class WHERE l1name='Nutrition'), 'Cooking'); 
INSERT INTO l2_class (l1id,l2name) VALUES ((SELECT l1id FROM l1_class WHERE l1name='Nutrition'), 'Financial'); 
INSERT INTO l2_class (l1id,l2name) VALUES ((SELECT l1id FROM l1_class WHERE l1name='Nutrition'), 'Facilities');

