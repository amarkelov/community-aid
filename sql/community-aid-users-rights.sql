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
GRANT ALL ON call_sclass_sclass_id_seq TO caadmin;
GRANT ALL ON calls_callid_seq TO caadmin;
GRANT ALL ON clients_clientid_seq TO caadmin;
GRANT ALL ON districts_districtid_seq TO caadmin;
GRANT ALL ON operators_operatorid_seq TO caadmin;
GRANT ALL ON groups_groupid_seq TO caadmin;
GRANT ALL ON l1_class_l1id_seq TO caadmin;
GRANT SELECT, INSERT, UPDATE, DELETE ON l1_class TO caadmin;
GRANT ALL ON l2_class_l2id_seq TO caadmin;
GRANT SELECT, INSERT, UPDATE, DELETE ON l2_class TO caadmin;

GRANT SELECT, INSERT, UPDATE, DELETE ON call_mclass TO caadmin;
GRANT SELECT, INSERT, UPDATE, DELETE ON call_sclass TO caadmin;
GRANT SELECT, INSERT, UPDATE ON calls TO caadmin;
GRANT SELECT, INSERT, UPDATE, DELETE ON client2operator TO caadmin;
GRANT SELECT, INSERT, UPDATE ON client_timeslot_call TO caadmin;
GRANT SELECT, INSERT, DELETE, UPDATE ON client_nextcalltime TO caadmin;
GRANT SELECT, INSERT, UPDATE ON clients TO caadmin;
GRANT SELECT, INSERT, UPDATE, DELETE ON days TO caadmin;
GRANT SELECT, INSERT, UPDATE, DELETE ON districts TO caadmin;
GRANT SELECT, INSERT, UPDATE, DELETE ON operators TO caadmin;
GRANT SELECT, INSERT, UPDATE, DELETE ON groups TO caadmin;
GRANT SELECT, INSERT, UPDATE, DELETE ON group2operator TO caadmin;
GRANT SELECT,INSERT,UPDATE,DELETE ON call_l2_class TO caadmin;

--
-- GRANT rights to default operator user
--
GRANT ALL ON calls_callid_seq TO caoperator;
GRANT SELECT ON call_mclass TO caoperator;
GRANT SELECT ON call_sclass TO caoperator;
GRANT SELECT, INSERT, UPDATE ON calls TO caoperator;
GRANT SELECT, INSERT, DELETE ON client2operator TO caoperator;
GRANT SELECT, INSERT, UPDATE ON client_timeslot_call TO caoperator;
GRANT SELECT, INSERT, DELETE, UPDATE ON client_nextcalltime TO caoperator;
GRANT SELECT ON clients TO caoperator;
GRANT SELECT ON days TO caoperator;
GRANT SELECT ON districts TO caoperator;
GRANT SELECT, UPDATE ON operators TO caoperator;
GRANT SELECT ON l1_class TO caoperator;
GRANT SELECT ON l2_class TO caoperator;
GRANT SELECT,INSERT ON call_l2_class TO caoperator;

--
-- Creating default user to fetch reports and giving rights
--

GRANT SELECT ON call_mclass TO careport;
GRANT SELECT ON call_sclass TO careport;
GRANT SELECT ON calls TO careport;
GRANT SELECT ON days TO careport;
GRANT SELECT ON client2operator TO careport;
GRANT SELECT ON client_timeslot_call TO careport;
GRANT SELECT ON clients TO careport;
GRANT SELECT ON districts TO careport;
GRANT SELECT ON operators TO careport;
GRANT SELECT ON l1_class TO careport;
GRANT SELECT ON l2_class TO careport;
GRANT SELECT ON call_l2_class TO careport;

