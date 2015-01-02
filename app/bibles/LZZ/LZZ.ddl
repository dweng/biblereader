
-- Table: info
CREATE TABLE info ( 
    name  TEXT,
    value TEXT 
);


-- Table: verses
CREATE TABLE verses ( 
    book_number NUMERIC,
    chapter     NUMERIC,
    verse       NUMERIC,
    text        TEXT 
);


-- Table: books
CREATE TABLE books ( 
    book_color    TEXT,
    book_number   NUMERIC,
    short_name    TEXT,
    long_name     TEXT,
    chapter_count NUMERIC 
);


-- Index: verses_index
CREATE UNIQUE INDEX verses_index ON verses ( 
    book_number,
    chapter,
    verse 
);

