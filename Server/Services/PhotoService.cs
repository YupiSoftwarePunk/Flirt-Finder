using Microsoft.EntityFrameworkCore;
using Server.Data;
using Server.Models;
using Server.Services.Interfaces;

namespace Server.Services
{
    public class PhotoService : IPhotoService
    {
        private readonly AppDbContext _context;

        public PhotoService(AppDbContext context)
        {
            _context = context;
        }

        public async Task<(byte[] Content, string ContentType)> GetPhotoByLoginAsync(string login)
        {
            var user = await _context.Users.FirstOrDefaultAsync(u => u.Login == login);
            if (user == null) return (null, null);

            var photo = await _context.Photos.FirstOrDefaultAsync(p => p.UserId == user.Id);
            if (photo == null || string.IsNullOrEmpty(photo.Url) || !File.Exists(photo.Url))
                return await GetDefaultPhotoAsync();

            var content = await File.ReadAllBytesAsync(photo.Url);
            return (content, "image/jpeg");
        }



        public async Task SavePhotoAsync(int userId, IFormFile file)
        {
            // Генерация уникального пути
            var uploadsFolder = Path.Combine(Directory.GetCurrentDirectory(), "Uploads");
            if (!Directory.Exists(uploadsFolder))
                Directory.CreateDirectory(uploadsFolder);

            var fileName = $"{Guid.NewGuid()}_{file.FileName}";
            var filePath = Path.Combine(uploadsFolder, fileName);

            // Сохранение файла
            using (var stream = new FileStream(filePath, FileMode.Create))
            {
                await file.CopyToAsync(stream);
            }

            // Удаление старого фото (если есть)
            var existing = await _context.Photos.FirstOrDefaultAsync(p => p.UserId == userId);
            if (existing != null)
            {
                if (!string.IsNullOrEmpty(existing.Url) && File.Exists(existing.Url))
                    File.Delete(existing.Url);

                _context.Photos.Remove(existing);
            }

            // Добавление нового фото
            var photo = new Photo
            {
                UserId = userId,
                Url = filePath
            };

            _context.Photos.Add(photo);
            await _context.SaveChangesAsync();
        }



        private async Task<(byte[] Content, string ContentType)> GetDefaultPhotoAsync()
        {
            var defaultPath = Path.Combine(Directory.GetCurrentDirectory(), "images", "default.png");
            var content = await File.ReadAllBytesAsync(defaultPath);
            return (content, "image/png");
        }


        public async Task<string> GetPhotoUrlByUserIdAsync(int userId)
        {
            var photo = await _context.Photos.FirstOrDefaultAsync(p => p.UserId == userId);
            return photo?.Url ?? "";
        }
    }
}
