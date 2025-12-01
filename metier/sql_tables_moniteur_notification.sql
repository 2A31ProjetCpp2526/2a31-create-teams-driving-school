-- ============================================================
-- SCRIPTS SQL POUR ORACLE DATABASE
-- Tables: MONITEUR et NOTIFICATION
-- Auto-école Management System
-- ============================================================

-- ============================================================
-- TABLE MONITEUR
-- ============================================================

-- Supprimer la table si elle existe (optionnel pour reset)
-- DROP TABLE MONITEUR CASCADE CONSTRAINTS;
-- DROP SEQUENCE SEQ_MONITEUR;

-- Créer la séquence pour l'ID auto-incrémenté
CREATE SEQUENCE SEQ_MONITEUR
    START WITH 1
    INCREMENT BY 1
    NOCACHE
    NOCYCLE;

-- Créer la table MONITEUR
CREATE TABLE MONITEUR (
    ID_MONITEUR     NUMBER PRIMARY KEY,
    CIN_MONITEUR    VARCHAR2(20) NOT NULL UNIQUE,
    NOM_MONITEUR    VARCHAR2(100) NOT NULL,
    PRENOM_MONITEUR VARCHAR2(100) NOT NULL,
    TEL_MONITEUR    VARCHAR2(20),
    EMAIL           VARCHAR2(150) UNIQUE,
    PERMIS          VARCHAR2(50) NOT NULL,  -- Types: A, B, C, D, E, etc.
    DATE_CREATION   DATE DEFAULT SYSDATE
);

-- Index pour améliorer les performances de recherche
CREATE INDEX IDX_MONITEUR_NOM ON MONITEUR(NOM_MONITEUR);
CREATE INDEX IDX_MONITEUR_PERMIS ON MONITEUR(PERMIS);
CREATE INDEX IDX_MONITEUR_EMAIL ON MONITEUR(EMAIL);

-- Commentaires sur les colonnes
COMMENT ON TABLE MONITEUR IS 'Table des moniteurs de l''auto-école';
COMMENT ON COLUMN MONITEUR.ID_MONITEUR IS 'Identifiant unique du moniteur (auto-généré)';
COMMENT ON COLUMN MONITEUR.CIN_MONITEUR IS 'Carte d''identité nationale (unique)';
COMMENT ON COLUMN MONITEUR.NOM_MONITEUR IS 'Nom de famille du moniteur';
COMMENT ON COLUMN MONITEUR.PRENOM_MONITEUR IS 'Prénom du moniteur';
COMMENT ON COLUMN MONITEUR.TEL_MONITEUR IS 'Numéro de téléphone';
COMMENT ON COLUMN MONITEUR.EMAIL IS 'Adresse email (unique)';
COMMENT ON COLUMN MONITEUR.PERMIS IS 'Type(s) de permis que le moniteur peut enseigner (A, B, C, D, E)';

-- Données de test
INSERT INTO MONITEUR (ID_MONITEUR, CIN_MONITEUR, NOM_MONITEUR, PRENOM_MONITEUR, TEL_MONITEUR, EMAIL, PERMIS)
VALUES (SEQ_MONITEUR.NEXTVAL, '12345678', 'Ben Ali', 'Mohamed', '98765432', 'benali.mohamed@email.com', 'B');

INSERT INTO MONITEUR (ID_MONITEUR, CIN_MONITEUR, NOM_MONITEUR, PRENOM_MONITEUR, TEL_MONITEUR, EMAIL, PERMIS)
VALUES (SEQ_MONITEUR.NEXTVAL, '87654321', 'Khalil', 'Fatima', '92345678', 'khalil.fatima@email.com', 'B');

INSERT INTO MONITEUR (ID_MONITEUR, CIN_MONITEUR, NOM_MONITEUR, PRENOM_MONITEUR, TEL_MONITEUR, EMAIL, PERMIS)
VALUES (SEQ_MONITEUR.NEXTVAL, '11223344', 'Trabelsi', 'Ahmed', '55667788', 'trabelsi.ahmed@email.com', 'A');

INSERT INTO MONITEUR (ID_MONITEUR, CIN_MONITEUR, NOM_MONITEUR, PRENOM_MONITEUR, TEL_MONITEUR, EMAIL, PERMIS)
VALUES (SEQ_MONITEUR.NEXTVAL, '44332211', 'Bouazizi', 'Sara', '99887766', 'bouazizi.sara@email.com', 'C');

INSERT INTO MONITEUR (ID_MONITEUR, CIN_MONITEUR, NOM_MONITEUR, PRENOM_MONITEUR, TEL_MONITEUR, EMAIL, PERMIS)
VALUES (SEQ_MONITEUR.NEXTVAL, '55667788', 'Mansour', 'Karim', '11223344', 'mansour.karim@email.com', 'D');

COMMIT;

-- ============================================================
-- TABLE NOTIFICATION
-- ============================================================

-- Supprimer la table si elle existe (optionnel pour reset)
-- DROP TABLE NOTIFICATION CASCADE CONSTRAINTS;
-- DROP SEQUENCE SEQ_NOTIFICATION;

-- Créer la séquence pour l'ID auto-incrémenté
CREATE SEQUENCE SEQ_NOTIFICATION
    START WITH 1
    INCREMENT BY 1
    NOCACHE
    NOCYCLE;

-- Créer la table NOTIFICATION
CREATE TABLE NOTIFICATION (
    ID_NOTIFICATION NUMBER PRIMARY KEY,
    TYPE_NOTIF      VARCHAR2(50) NOT NULL,      -- AJOUT, MODIFICATION, SUPPRESSION
    MESSAGE         VARCHAR2(500) NOT NULL,
    ENTITE          VARCHAR2(50),               -- EMPLOYE, MONITEUR, CLIENT, VEHICULE, SEANCE, CIRCUIT
    DATE_HEURE      TIMESTAMP DEFAULT SYSTIMESTAMP,
    LUE             NUMBER(1) DEFAULT 0         -- 0 = non lue, 1 = lue
);

-- Contrainte CHECK pour le type de notification
ALTER TABLE NOTIFICATION ADD CONSTRAINT CHK_TYPE_NOTIF 
    CHECK (TYPE_NOTIF IN ('AJOUT', 'MODIFICATION', 'SUPPRESSION', 'ALERTE', 'INFO'));

-- Contrainte CHECK pour l'entité
ALTER TABLE NOTIFICATION ADD CONSTRAINT CHK_ENTITE 
    CHECK (ENTITE IN ('EMPLOYE', 'MONITEUR', 'CLIENT', 'VEHICULE', 'SEANCE', 'CIRCUIT', NULL));

-- Contrainte CHECK pour le champ LUE
ALTER TABLE NOTIFICATION ADD CONSTRAINT CHK_LUE 
    CHECK (LUE IN (0, 1));

-- Index pour améliorer les performances
CREATE INDEX IDX_NOTIFICATION_DATE ON NOTIFICATION(DATE_HEURE DESC);
CREATE INDEX IDX_NOTIFICATION_TYPE ON NOTIFICATION(TYPE_NOTIF);
CREATE INDEX IDX_NOTIFICATION_ENTITE ON NOTIFICATION(ENTITE);
CREATE INDEX IDX_NOTIFICATION_LUE ON NOTIFICATION(LUE);

-- Commentaires sur les colonnes
COMMENT ON TABLE NOTIFICATION IS 'Table des notifications système pour l''auto-école';
COMMENT ON COLUMN NOTIFICATION.ID_NOTIFICATION IS 'Identifiant unique de la notification';
COMMENT ON COLUMN NOTIFICATION.TYPE_NOTIF IS 'Type de notification (AJOUT, MODIFICATION, SUPPRESSION, ALERTE, INFO)';
COMMENT ON COLUMN NOTIFICATION.MESSAGE IS 'Message descriptif de la notification';
COMMENT ON COLUMN NOTIFICATION.ENTITE IS 'Entité concernée (EMPLOYE, MONITEUR, CLIENT, VEHICULE, SEANCE, CIRCUIT)';
COMMENT ON COLUMN NOTIFICATION.DATE_HEURE IS 'Date et heure de création de la notification';
COMMENT ON COLUMN NOTIFICATION.LUE IS 'Indicateur si la notification a été lue (0=non, 1=oui)';

-- Données de test
INSERT INTO NOTIFICATION (ID_NOTIFICATION, TYPE_NOTIF, MESSAGE, ENTITE, DATE_HEURE, LUE)
VALUES (SEQ_NOTIFICATION.NEXTVAL, 'AJOUT', 'Nouveau moniteur ajouté: Ben Ali Mohamed', 'MONITEUR', SYSTIMESTAMP, 0);

INSERT INTO NOTIFICATION (ID_NOTIFICATION, TYPE_NOTIF, MESSAGE, ENTITE, DATE_HEURE, LUE)
VALUES (SEQ_NOTIFICATION.NEXTVAL, 'AJOUT', 'Nouveau client ajouté: Dupont Jean', 'CLIENT', SYSTIMESTAMP - INTERVAL '1' HOUR, 0);

INSERT INTO NOTIFICATION (ID_NOTIFICATION, TYPE_NOTIF, MESSAGE, ENTITE, DATE_HEURE, LUE)
VALUES (SEQ_NOTIFICATION.NEXTVAL, 'MODIFICATION', 'Véhicule modifié: 123TU1234', 'VEHICULE', SYSTIMESTAMP - INTERVAL '2' HOUR, 1);

COMMIT;

-- ============================================================
-- VUES UTILES (optionnel)
-- ============================================================

-- Vue pour les notifications non lues
CREATE OR REPLACE VIEW V_NOTIFICATIONS_NON_LUES AS
SELECT ID_NOTIFICATION, TYPE_NOTIF, MESSAGE, ENTITE, DATE_HEURE
FROM NOTIFICATION
WHERE LUE = 0
ORDER BY DATE_HEURE DESC;

-- Vue pour les statistiques des moniteurs par permis
CREATE OR REPLACE VIEW V_STATS_MONITEUR_PERMIS AS
SELECT PERMIS, COUNT(*) AS NB_MONITEURS
FROM MONITEUR
GROUP BY PERMIS
ORDER BY NB_MONITEURS DESC;

-- ============================================================
-- PROCÉDURES STOCKÉES (optionnel)
-- ============================================================

-- Procédure pour marquer toutes les notifications comme lues
CREATE OR REPLACE PROCEDURE SP_MARQUER_NOTIFS_LUES AS
BEGIN
    UPDATE NOTIFICATION SET LUE = 1 WHERE LUE = 0;
    COMMIT;
END;
/

-- Procédure pour supprimer les anciennes notifications (plus de 30 jours)
CREATE OR REPLACE PROCEDURE SP_PURGER_ANCIENNES_NOTIFS AS
BEGIN
    DELETE FROM NOTIFICATION WHERE DATE_HEURE < SYSTIMESTAMP - INTERVAL '30' DAY;
    COMMIT;
END;
/

-- ============================================================
-- FIN DU SCRIPT
-- ============================================================
