using Microsoft.EntityFrameworkCore;
using Server.Data;
using Server.DTOs;
using Server.Models;
using Server.Services.Interfaces;

namespace Server.Services
{
    public class UserService : IUserService
    {
        private readonly AppDbContext _context;
        private readonly IPhotoService _photoService;

        public UserService(AppDbContext context, IPhotoService photoService)
        {
            _context = context;
            _photoService = photoService;
        }

        public async Task<UserDto> GetByIdAsync(int id)
        {
            var user = await _context.Users.FindAsync(id);
            if (user == null) return null;

            return new UserDto
            {
                Id = user.Id,
                Username = user.Username,
                Bio = user.Bio,
                Gender = user.Gender,
                Login = user.Login,
                City = user.City,
                Age = user.Age
            };
        }

        public async Task<bool> UpdateAsync(int id, UpdateUserDto dto)
        {
            var user = await _context.Users.FindAsync(id);
            if (user == null) return false;

            user.Bio = dto.Bio;
            user.Gender = dto.Gender;
            user.Age = dto.Age;
            user.City = dto.City;

            await _context.SaveChangesAsync();
            return true;
        }

        public async Task<bool> PatchAsync(int id, PatchUserDto dto)
        {
            var user = await _context.Users.FindAsync(id);
            if (user == null) return false;

            if (dto.Bio != null) user.Bio = dto.Bio;

            await _context.SaveChangesAsync();
            return true;
        }


        public async Task<(bool Success, string Message, UserDto User)> CreateAsync(CreateUserDto dto)
        {
            var existing = await _context.Users.FirstOrDefaultAsync(u => u.Username == dto.Username);
            if (existing != null)
            {
                return (false, "Пользователь с таким именем уже существует", null);
            }

            var user = new User
            {
                Username = dto.Username,
                Bio = dto.Bio,
                Gender = dto.Gender,
                Age = dto.Age,
                Login = dto.Login,
                City = dto.City,
                Password = BCrypt.Net.BCrypt.HashPassword((string)dto.Password)
            };

            _context.Users.Add(user);
            await _context.SaveChangesAsync();

            var userDto = new UserDto
            {
                Id = user.Id,
                Username = user.Username,
                Bio = user.Bio,
                Gender = user.Gender,
                Age = user.Age
            };

            return (true, "Пользователь создан", userDto);
        }



        public async Task<IEnumerable<UserProfileDto>> GetProfilesAsync(int excludeUserId, string gender)
        {
            var users = await _context.Users
            .Where(u => u.Id != excludeUserId && u.Gender == gender)
            .ToListAsync();

            var profiles = new List<UserProfileDto>();

            foreach (var user in users)
            {
                var photoUrl = await _photoService.GetPhotoUrlByUserIdAsync(user.Id); 

                profiles.Add(new UserProfileDto
                {
                    Id = user.Id,
                    FullName = user.Username,
                    Age = user.Age,
                    City = user.City,
                    Bio = user.Bio,
                    Gender = user.Gender,
                    PhotoUrl = photoUrl
                });
            }

            return profiles;
        }


        public async Task<User?> GetByLoginAsync(string login)
        {
            return await _context.Users.FirstOrDefaultAsync(u => u.Login == login);
        }
    }
}
