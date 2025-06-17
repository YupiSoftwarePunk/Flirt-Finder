CREATE TABLE photos (
    id SERIAL PRIMARY KEY,
    user_id INTEGER NOT NULL,
    photo_path TEXT NOT NULL DEFAULT '',
    FOREIGN KEY (user_id) REFERENCES users (id) ON DELETE CASCADE
);
