using Microsoft.EntityFrameworkCore;
using Server.Data;
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
            if (photo == null || string.IsNullOrEmpty(photo.Url)) return (null, null);

            var content = await File.ReadAllBytesAsync(photo.Url);
            return (content, "image/jpeg");
        }
    }
}
