CREATE TABLE users (
    id SERIAL PRIMARY KEY,
    login TEXT UNIQUE NOT NULL,
    password TEXT NOT NULL,
    name TEXT NOT NULL,
    gender TEXT,
    age INTEGER,
    hobbies TEXT,
    city TEXT
);
