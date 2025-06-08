CREATE TABLE users (
    id SERIAL PRIMARY KEY,
    login TEXT UNIQUE NOT NULL,
    password TEXT NOT NULL,
    name TEXT NOT NULL DEFAULT '',
    gender TEXT DEFAULT '',
    age INTEGER DEFAULT 16,
    hobbies TEXT DEFAULT '',
    city TEXT DEFAULT ''
);
