CREATE TABLE likes_dislikes (
    id SERIAL PRIMARY KEY,
    user_id INTEGER NOT NULL, -- Кому поставили лайк/дизлайк
    liked_by INTEGER NOT NULL, -- Кто поставил
    reaction INTEGER NOT NULL, -- 1 = лайк, -1 = дизлайк
    FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE CASCADE,
    FOREIGN KEY (liked_by) REFERENCES users(id) ON DELETE CASCADE
);

ALTER TABLE likes_dislikes
ADD CONSTRAINT unique_reaction UNIQUE (user_id, liked_by);
