-- ============================================================
-- Smart Research Publication Management
-- FULL RESET + SEED SCRIPT  (Oracle 11g compatible)
-- Run in SQL Developer as: dhia
-- Press F5 (Run Script) - NOT F9
-- ============================================================

SET DEFINE OFF
SET FEEDBACK ON

-- ============================================================
-- STEP 1: DROP TABLES with CASCADE CONSTRAINTS
-- ============================================================
BEGIN EXECUTE IMMEDIATE 'DROP TABLE Evaluation   CASCADE CONSTRAINTS'; EXCEPTION WHEN OTHERS THEN NULL; END;
/
BEGIN EXECUTE IMMEDIATE 'DROP TABLE SOUMISSION   CASCADE CONSTRAINTS'; EXCEPTION WHEN OTHERS THEN NULL; END;
/
BEGIN EXECUTE IMMEDIATE 'DROP TABLE ARTICLE      CASCADE CONSTRAINTS'; EXCEPTION WHEN OTHERS THEN NULL; END;
/
BEGIN EXECUTE IMMEDIATE 'DROP TABLE CONFERENCE   CASCADE CONSTRAINTS'; EXCEPTION WHEN OTHERS THEN NULL; END;
/
BEGIN EXECUTE IMMEDIATE 'DROP TABLE LABORATOIRE  CASCADE CONSTRAINTS'; EXCEPTION WHEN OTHERS THEN NULL; END;
/
BEGIN EXECUTE IMMEDIATE 'DROP TABLE USERS        CASCADE CONSTRAINTS'; EXCEPTION WHEN OTHERS THEN NULL; END;
/
BEGIN EXECUTE IMMEDIATE 'DROP TABLE Reviewer     CASCADE CONSTRAINTS'; EXCEPTION WHEN OTHERS THEN NULL; END;
/

-- ============================================================
-- STEP 2: DROP SEQUENCES
-- ============================================================
BEGIN EXECUTE IMMEDIATE 'DROP SEQUENCE SEQ_USER';        EXCEPTION WHEN OTHERS THEN NULL; END;
/
BEGIN EXECUTE IMMEDIATE 'DROP SEQUENCE SEQ_LABO';        EXCEPTION WHEN OTHERS THEN NULL; END;
/
BEGIN EXECUTE IMMEDIATE 'DROP SEQUENCE SEQ_ARTICLE';     EXCEPTION WHEN OTHERS THEN NULL; END;
/
BEGIN EXECUTE IMMEDIATE 'DROP SEQUENCE SEQ_CONFERENCE';  EXCEPTION WHEN OTHERS THEN NULL; END;
/
BEGIN EXECUTE IMMEDIATE 'DROP SEQUENCE SEQ_SOUMISSION';  EXCEPTION WHEN OTHERS THEN NULL; END;
/
BEGIN EXECUTE IMMEDIATE 'DROP SEQUENCE SEQ_EVALUATION';  EXCEPTION WHEN OTHERS THEN NULL; END;
/

-- ============================================================
-- STEP 3: CREATE SEQUENCES
-- ============================================================
CREATE SEQUENCE SEQ_USER       START WITH 1 INCREMENT BY 1 NOCACHE NOCYCLE;
CREATE SEQUENCE SEQ_LABO       START WITH 1 INCREMENT BY 1 NOCACHE NOCYCLE;
CREATE SEQUENCE SEQ_ARTICLE    START WITH 1 INCREMENT BY 1 NOCACHE NOCYCLE;
CREATE SEQUENCE SEQ_CONFERENCE START WITH 1 INCREMENT BY 1 NOCACHE NOCYCLE;
CREATE SEQUENCE SEQ_SOUMISSION START WITH 1 INCREMENT BY 1 NOCACHE NOCYCLE;
CREATE SEQUENCE SEQ_EVALUATION START WITH 1 INCREMENT BY 1 NOCACHE NOCYCLE;

-- ============================================================
-- STEP 4: CREATE TABLES  (NO / after CREATE TABLE)
-- ============================================================

CREATE TABLE USERS (
    ID_user  NUMBER        PRIMARY KEY,
    nom      VARCHAR2(100) NOT NULL,
    prenom   VARCHAR2(100) NOT NULL,
    email    VARCHAR2(150) UNIQUE NOT NULL,
    mdp      VARCHAR2(255) NOT NULL,
    Role     VARCHAR2(50),
    photo    VARCHAR2(255)
);

CREATE OR REPLACE TRIGGER TRG_USERS
BEFORE INSERT ON USERS FOR EACH ROW
BEGIN
    IF :NEW.ID_user IS NULL THEN
        SELECT SEQ_USER.NEXTVAL INTO :NEW.ID_user FROM DUAL;
    END IF;
END;
/

CREATE TABLE LABORATOIRE (
    ID_labo           NUMBER        PRIMARY KEY,
    nom               VARCHAR2(200) NOT NULL,
    budget            NUMBER(15,2),
    Date_creation     DATE          NOT NULL,
    domaine           VARCHAR2(100) NOT NULL,
    localisation      VARCHAR2(200) NOT NULL,
    nombre_chercheurs NUMBER        DEFAULT 0,
    classement        VARCHAR2(50),
    historique        CLOB,
    ID_user           NUMBER,
    CONSTRAINT FK_Lab_User FOREIGN KEY (ID_user) REFERENCES USERS(ID_user)
);

CREATE OR REPLACE TRIGGER TRG_LABORATOIRE
BEFORE INSERT ON LABORATOIRE FOR EACH ROW
BEGIN
    IF :NEW.ID_labo IS NULL THEN
        SELECT SEQ_LABO.NEXTVAL INTO :NEW.ID_labo FROM DUAL;
    END IF;
END;
/

CREATE TABLE ARTICLE (
    ID_article    NUMBER        PRIMARY KEY,
    titre         VARCHAR2(255) NOT NULL,
    contenu       CLOB,
    domaine       VARCHAR2(100) NOT NULL,
    Etat          VARCHAR2(50)  NOT NULL,
    Date_creation DATE          DEFAULT SYSDATE,
    ID_user       NUMBER,
    CONSTRAINT FK_Article_User FOREIGN KEY (ID_user) REFERENCES USERS(ID_user)
);

CREATE OR REPLACE TRIGGER TRG_ARTICLE
BEFORE INSERT ON ARTICLE FOR EACH ROW
BEGIN
    IF :NEW.ID_article IS NULL THEN
        SELECT SEQ_ARTICLE.NEXTVAL INTO :NEW.ID_article FROM DUAL;
    END IF;
END;
/

CREATE TABLE CONFERENCE (
    ID_conference NUMBER        PRIMARY KEY,
    nom           VARCHAR2(255) NOT NULL,
    lieu          VARCHAR2(150) NOT NULL,
    Date_debut    DATE          NOT NULL,
    Date_fin      DATE,
    frais_inscri  NUMBER(10,2),
    etat          VARCHAR2(50)  NOT NULL,
    theme         VARCHAR2(100) NOT NULL
);

CREATE OR REPLACE TRIGGER TRG_CONFERENCE
BEFORE INSERT ON CONFERENCE FOR EACH ROW
BEGIN
    IF :NEW.ID_conference IS NULL THEN
        SELECT SEQ_CONFERENCE.NEXTVAL INTO :NEW.ID_conference FROM DUAL;
    END IF;
END;
/

CREATE TABLE SOUMISSION (
    ID_soumission     NUMBER       PRIMARY KEY,
    ID_article        NUMBER       NOT NULL,
    ID_conference     NUMBER,
    ID_user           NUMBER,
    Date_envoi        DATE         DEFAULT SYSDATE,
    Date_lim_revision DATE,
    Etat              VARCHAR2(50) NOT NULL,
    Decision_finale   VARCHAR2(50),
    Date_decision     DATE,
    CONSTRAINT FK_Soum_Article FOREIGN KEY (ID_article)    REFERENCES ARTICLE(ID_article),
    CONSTRAINT FK_Soum_Conf    FOREIGN KEY (ID_conference) REFERENCES CONFERENCE(ID_conference),
    CONSTRAINT FK_Soum_User    FOREIGN KEY (ID_user)       REFERENCES USERS(ID_user)
);

CREATE OR REPLACE TRIGGER TRG_SOUMISSION
BEFORE INSERT ON SOUMISSION FOR EACH ROW
BEGIN
    IF :NEW.ID_soumission IS NULL THEN
        SELECT SEQ_SOUMISSION.NEXTVAL INTO :NEW.ID_soumission FROM DUAL;
    END IF;
END;
/

CREATE TABLE Evaluation (
    id_evaluation   NUMBER       PRIMARY KEY,
    soumission_id   NUMBER       NOT NULL,
    relecteur_id    NUMBER       NOT NULL,
    note            NUMBER(2)    CHECK (note BETWEEN 1 AND 10),
    commentaires    CLOB,
    date_evaluation DATE         DEFAULT SYSDATE,
    statut          VARCHAR2(20) DEFAULT 'en_cours'
                    CHECK (statut IN ('en_cours', 'terminee')),
    CONSTRAINT FK_Eval_Soum FOREIGN KEY (soumission_id) REFERENCES SOUMISSION(ID_soumission),
    CONSTRAINT FK_Eval_User FOREIGN KEY (relecteur_id)  REFERENCES USERS(ID_user)
);

CREATE OR REPLACE TRIGGER TRG_EVALUATION
BEFORE INSERT ON Evaluation FOR EACH ROW
BEGIN
    IF :NEW.id_evaluation IS NULL THEN
        SELECT SEQ_EVALUATION.NEXTVAL INTO :NEW.id_evaluation FROM DUAL;
    END IF;
END;
/

-- ============================================================
-- STEP 5: SEED DATA  (NO / after INSERT)
-- ============================================================

-- ── USERS ─────────────────────────────────────────────────────
-- 1 Admin
INSERT INTO USERS (nom, prenom, email, mdp, Role) VALUES ('Admin',    'System',  'admin@smartresearch.tn',    'admin', 'Admin');
-- 3 Chercheurs
INSERT INTO USERS (nom, prenom, email, mdp, Role) VALUES ('Ben Ali',  'Ahmed',   'ahmed.benali@univ.tn',      '1234',  'Enseignant');
INSERT INTO USERS (nom, prenom, email, mdp, Role) VALUES ('Khalil',   'Mohamed', 'mohamed.khalil@enis.tn',    '1234',  'Enseignant');
INSERT INTO USERS (nom, prenom, email, mdp, Role) VALUES ('Zahra',    'Fatima',  'fatima.zahra@esprit.tn',    '1234',  'Enseignant');
-- 3 Relecteurs
INSERT INTO USERS (nom, prenom, email, mdp, Role) VALUES ('Trabelsi', 'Sonia',   'sonia.trabelsi@insat.tn',   '1234',  'Relecteur');
INSERT INTO USERS (nom, prenom, email, mdp, Role) VALUES ('Dupont',   'Jean',    'jean.dupont@sorbonne.fr',   '1234',  'Relecteur');
INSERT INTO USERS (nom, prenom, email, mdp, Role) VALUES ('Martin',   'Sarah',   'sarah.martin@epfl.ch',      '1234',  'Relecteur');
-- 1 Directeur Labo
INSERT INTO USERS (nom, prenom, email, mdp, Role) VALUES ('Mansour',  'Karim',   'karim.mansour@utm.tn',      '1234',  'Directeur de Laboratoire');

COMMIT;

-- ── LABORATOIRES ──────────────────────────────────────────────
INSERT INTO LABORATOIRE (nom, budget, Date_creation, domaine, localisation, nombre_chercheurs, classement, ID_user)
VALUES ('Laboratoire IA et Big Data', 150000, TO_DATE('2015-01-10','YYYY-MM-DD'), 'Intelligence Artificielle', 'Tunis, INSAT', 12, 'A+', 8);

INSERT INTO LABORATOIRE (nom, budget, Date_creation, domaine, localisation, nombre_chercheurs, classement, ID_user)
VALUES ('Labo Systemes Embarques', 80000, TO_DATE('2018-03-15','YYYY-MM-DD'), 'Electronique', 'Sfax, ENIS', 7, 'B', 8);

COMMIT;

-- ── CONFERENCES ───────────────────────────────────────────────
INSERT INTO CONFERENCE (nom, lieu, Date_debut, Date_fin, frais_inscri, etat, theme)
VALUES ('International Conference on AI 2025', 'Tunis, Centre des Congres',
        TO_DATE('2025-06-10','YYYY-MM-DD'), TO_DATE('2025-06-12','YYYY-MM-DD'), 250, 'Ouverte', 'Intelligence Artificielle');

INSERT INTO CONFERENCE (nom, lieu, Date_debut, Date_fin, frais_inscri, etat, theme)
VALUES ('Cybersecurity Summit Maghreb 2025', 'Sousse, Palais des Congres',
        TO_DATE('2025-09-20','YYYY-MM-DD'), TO_DATE('2025-09-22','YYYY-MM-DD'), 300, 'Ouverte', 'Cybersecurite');

INSERT INTO CONFERENCE (nom, lieu, Date_debut, Date_fin, frais_inscri, etat, theme)
VALUES ('Big Data et Cloud Expo 2024', 'Sfax, Universite',
        TO_DATE('2024-11-05','YYYY-MM-DD'), TO_DATE('2024-11-07','YYYY-MM-DD'), 150, 'Terminee', 'Big Data');

COMMIT;

-- ── ARTICLES ──────────────────────────────────────────────────
-- par Ahmed Ben Ali  (ID_user=2)
INSERT INTO ARTICLE (titre, domaine, Etat, Date_creation, ID_user)
VALUES ('Deep Learning pour la Detection de Maladies Cardiaques', 'Intelligence Artificielle', 'Soumis', TO_DATE('2025-01-15','YYYY-MM-DD'), 2);

INSERT INTO ARTICLE (titre, domaine, Etat, Date_creation, ID_user)
VALUES ('Optimisation des Reseaux de Neurones pour Systemes Embarques', 'Intelligence Artificielle', 'Brouillon', TO_DATE('2025-02-20','YYYY-MM-DD'), 2);

-- par Mohamed Khalil (ID_user=3)
INSERT INTO ARTICLE (titre, domaine, Etat, Date_creation, ID_user)
VALUES ('Blockchain pour la Securite des Transactions Medicales', 'Cybersecurite', 'Soumis', TO_DATE('2025-01-28','YYYY-MM-DD'), 3);

-- par Fatima Zahra   (ID_user=4)
INSERT INTO ARTICLE (titre, domaine, Etat, Date_creation, ID_user)
VALUES ('Energies Renouvelables et Intelligence Artificielle', 'Intelligence Artificielle', 'Publie', TO_DATE('2024-10-10','YYYY-MM-DD'), 4);

INSERT INTO ARTICLE (titre, domaine, Etat, Date_creation, ID_user)
VALUES ('Cloud Computing pour les Systemes de Sante en Tunisie', 'Cloud Computing', 'Soumis', TO_DATE('2025-03-01','YYYY-MM-DD'), 4);

COMMIT;

-- ── SOUMISSIONS ───────────────────────────────────────────────
-- Article 1 -> Conference 1 (IA Tunis) par Ahmed (ID_user=2)
INSERT INTO SOUMISSION (ID_article, ID_conference, ID_user, Date_envoi, Etat)
VALUES (1, 1, 2, TO_DATE('2025-02-01','YYYY-MM-DD'), 'En revision');

-- Article 3 -> Conference 2 (Cybersecurite) par Mohamed (ID_user=3)
INSERT INTO SOUMISSION (ID_article, ID_conference, ID_user, Date_envoi, Etat)
VALUES (3, 2, 3, TO_DATE('2025-02-15','YYYY-MM-DD'), 'En attente');

-- Article 4 -> Conference 3 (Big Data 2024) par Fatima (ID_user=4)
INSERT INTO SOUMISSION (ID_article, ID_conference, ID_user, Date_envoi, Etat, Decision_finale, Date_decision)
VALUES (4, 3, 4, TO_DATE('2024-09-20','YYYY-MM-DD'), 'Accepte', 'Accepte', TO_DATE('2024-10-01','YYYY-MM-DD'));

-- Article 5 -> Conference 1 (IA Tunis) par Fatima (ID_user=4)
INSERT INTO SOUMISSION (ID_article, ID_conference, ID_user, Date_envoi, Etat)
VALUES (5, 1, 4, TO_DATE('2025-03-05','YYYY-MM-DD'), 'En attente');

COMMIT;

-- ── EVALUATIONS ───────────────────────────────────────────────
-- Soumission 1 evaluee par Sonia Trabelsi (ID_user=5)
INSERT INTO Evaluation (soumission_id, relecteur_id, note, commentaires, date_evaluation, statut)
VALUES (1, 5, 8, 'Tres bon article. Methodologie solide et resultats convaincants. Ameliorations mineures en bibliographie.', TO_DATE('2025-02-20','YYYY-MM-DD'), 'terminee');

-- Soumission 2 evaluee par Jean Dupont (ID_user=6)
INSERT INTO Evaluation (soumission_id, relecteur_id, note, commentaires, date_evaluation, statut)
VALUES (2, 6, 6, 'Article interessant mais manque de profondeur theorique. Implementation bien detaillee. A ameliorer.', TO_DATE('2025-03-01','YYYY-MM-DD'), 'en_cours');

-- Soumission 2 evaluee aussi par Sarah Martin (ID_user=7)
INSERT INTO Evaluation (soumission_id, relecteur_id, note, commentaires, date_evaluation, statut)
VALUES (2, 7, 7, 'Bonne contribution. Experiences bien conçues. Conclusion a developper davantage.', TO_DATE('2025-03-05','YYYY-MM-DD'), 'terminee');

-- Soumission 3 evaluee par Sonia Trabelsi
INSERT INTO Evaluation (soumission_id, relecteur_id, note, commentaires, date_evaluation, statut)
VALUES (3, 5, 9, 'Excellent article. Contribution originale et tres bien redige. Recommande pour publication.', TO_DATE('2024-10-05','YYYY-MM-DD'), 'terminee');

COMMIT;

-- ============================================================
-- VERIFICATION FINALE
-- ============================================================
SELECT 'USERS'       AS "Table",  COUNT(*) AS "Lignes" FROM USERS       UNION ALL
SELECT 'LABORATOIRE',              COUNT(*)             FROM LABORATOIRE  UNION ALL
SELECT 'CONFERENCE',               COUNT(*)             FROM CONFERENCE   UNION ALL
SELECT 'ARTICLE',                  COUNT(*)             FROM ARTICLE      UNION ALL
SELECT 'SOUMISSION',               COUNT(*)             FROM SOUMISSION   UNION ALL
SELECT 'Evaluation',               COUNT(*)             FROM Evaluation;
