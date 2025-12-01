-- 🔹 Supprimer la table si elle existe déjà
BEGIN
    EXECUTE IMMEDIATE 'DROP TABLE ADMIN.MONITEUR CASCADE CONSTRAINTS';
EXCEPTION
    WHEN OTHERS THEN
        IF SQLCODE != -942 THEN RAISE; END IF;
END;
/

-- 🔹 Supprimer la séquence si elle existe déjà
BEGIN
    EXECUTE IMMEDIATE 'DROP SEQUENCE ADMIN.SEQ_MONITEUR';
EXCEPTION
    WHEN OTHERS THEN
        IF SQLCODE != -2289 THEN RAISE; END IF;
END;
/

-- 🔹 Supprimer le trigger si il existe déjà
BEGIN
    EXECUTE IMMEDIATE 'DROP TRIGGER ADMIN.TRIG_MONITEUR_ID';
EXCEPTION
    WHEN OTHERS THEN
        IF SQLCODE != -4080 THEN RAISE; END IF;
END;
/

-- ============================================================
-- ✅ Création de la table MONITEUR dans le schéma ADMIN
-- ============================================================

CREATE TABLE ADMIN.MONITEUR (
    ID_MONITEUR     NUMBER PRIMARY KEY,
    CIN_MONITEUR    VARCHAR2(20 BYTE) NOT NULL,
    NOM_MONITEUR    VARCHAR2(20 BYTE) NOT NULL,
    PRENOM_MONITEUR VARCHAR2(20 BYTE) NOT NULL,
    TEL_MONITEUR    NUMBER(15,0),
    EMAIL           VARCHAR2(50 BYTE),
    PERMIS          VARCHAR2(20 BYTE)
)
TABLESPACE SYSTEM
LOGGING
NOCOMPRESS;
/

-- ============================================================
-- ✅ Séquence pour générer automatiquement les ID
-- ============================================================

CREATE SEQUENCE ADMIN.SEQ_MONITEUR
    START WITH 1
    INCREMENT BY 1
    NOCACHE
    NOCYCLE;
/

-- ============================================================
-- ✅ Trigger d’incrémentation automatique de l’ID
-- ============================================================

CREATE OR REPLACE TRIGGER ADMIN.TRIG_MONITEUR_ID
BEFORE INSERT ON ADMIN.MONITEUR
FOR EACH ROW
BEGIN
    IF :NEW.ID_MONITEUR IS NULL THEN
        SELECT ADMIN.SEQ_MONITEUR.NEXTVAL INTO :NEW.ID_MONITEUR FROM dual;
    END IF;
END;
/
