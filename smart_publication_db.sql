

-- Créer les séquences pour auto-increment
CREATE SEQUENCE SEQ_USER START WITH 1 INCREMENT BY 1;
CREATE SEQUENCE SEQ_LABO START WITH 1 INCREMENT BY 1;
CREATE SEQUENCE SEQ_SOUMISSION START WITH 1 INCREMENT BY 1;
CREATE SEQUENCE SEQ_ARTICLE START WITH 1 INCREMENT BY 1;
CREATE SEQUENCE SEQ_CONFERENCE START WITH 1 INCREMENT BY 1;
CREATE SEQUENCE SEQ_EVALUATION START WITH 1 INCREMENT BY 1;

-- ============================================
-- TABLE: USER
-- ============================================
CREATE TABLE USERS (
    ID_user NUMBER PRIMARY KEY,
    nom VARCHAR2(100) NOT NULL,
    prenom VARCHAR2(100) NOT NULL,
    email VARCHAR2(150) UNIQUE NOT NULL,
    mdp VARCHAR2(255) NOT NULL,
    Role VARCHAR2(50),
    photo VARCHAR2(255)
);

-- ============================================
-- TABLE: LABORATOIRE
-- ============================================
CREATE TABLE LABORATOIRE (
    ID_labo NUMBER PRIMARY KEY,
    nom VARCHAR2(200) NOT NULL,
    budget NUMBER(15,2),
    Date_creation DATE NOT NULL,
    domaine VARCHAR2(100) NOT NULL,
    localisation VARCHAR2(200) NOT NULL,
    nombre_chercheurs NUMBER DEFAULT 0,
    classement VARCHAR2(50),
    historique CLOB,
    ID_user NUMBER,
    CONSTRAINT FK_Lab_User FOREIGN KEY (ID_user) REFERENCES USERS(ID_user)
);

-- ============================================
-- TABLE: SOUMISSION
-- ============================================
CREATE TABLE SOUMISSION (
    ID_soumission NUMBER PRIMARY KEY,
    Date_envoi DATE DEFAULT SYSDATE,
    Date_lim_revision DATE,
    Etat VARCHAR2(50) NOT NULL,
    Decision_finale VARCHAR2(50),
    Date_decision DATE
);

-- ============================================
-- TABLE: ARTICLE
-- ============================================
CREATE TABLE ARTICLE (
    ID_article NUMBER PRIMARY KEY,
    titre VARCHAR2(255) NOT NULL,
    contenu CLOB,
    domaine VARCHAR2(100) NOT NULL,
    Etat VARCHAR2(50) NOT NULL,
    Date_creation DATE DEFAULT SYSDATE,
    ID_user NUMBER,
    ID_soumission NUMBER,
    CONSTRAINT FK_Article_User FOREIGN KEY (ID_user) REFERENCES USERS(ID_user),
    CONSTRAINT FK_Article_Soum FOREIGN KEY (ID_soumission) REFERENCES SOUMISSION(ID_soumission)
);

-- ============================================
-- TABLE: CONFERENCE
-- ============================================
CREATE TABLE CONFERENCE (
    ID_conference NUMBER PRIMARY KEY,
    nom VARCHAR2(255) NOT NULL,
    lieu VARCHAR2(150) NOT NULL,
    Date_debut DATE NOT NULL,
    Date_fin DATE,
    frais_inscri NUMBER(10,2),
    etat VARCHAR2(50) NOT NULL,
    theme VARCHAR2(100) NOT NULL,
    ID_article NUMBER,
    CONSTRAINT FK_Conf_Article FOREIGN KEY (ID_article) REFERENCES ARTICLE(ID_article)
);

-- ============================================
-- TABLE: EVALUATION
-- ============================================
CREATE TABLE EVALUATION (
    ID_evaluation NUMBER PRIMARY KEY,
    note NUMBER(4,2),
    commentaires CLOB,
    Date_evaluation DATE DEFAULT SYSDATE,
    statut VARCHAR2(50) NOT NULL,
    deadline DATE,
    ID_soumission NUMBER,
    CONSTRAINT FK_Eval_Soum FOREIGN KEY (ID_soumission) REFERENCES SOUMISSION(ID_soumission)
);

-- Message de confirmation
BEGIN
    DBMS_OUTPUT.PUT_LINE('Base de données créée avec succès!');
END;
/
