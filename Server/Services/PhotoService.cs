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
            var user = await _context.Users.FirstOrDefaultAsync(u => u.Username == login);
            if (user == null || string.IsNullOrEmpty(user.PhotoUrl)) return (null, null);

            var content = await File.ReadAllBytesAsync(user.PhotoUrl);
            return (content, "image/jpeg");
        }
    }
}
