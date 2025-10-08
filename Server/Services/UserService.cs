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

        public UserService(AppDbContext context)
        {
            _context = context;
        }

        public async Task<UserDto> GetByIdAsync(string id)
        {
            var user = await _context.Users.FindAsync(id);
            if (user == null) return null;

            return new UserDto
            {
                Id = user.Id,
                Username = user.Username,
                Bio = user.Bio,
                Gender = user.Gender,
                BirthDate = user.BirthDate,
                PhotoUrl = user.PhotoUrl,
            };
        }

        public async Task<bool> UpdateAsync(string id, UpdateUserDto dto)
        {
            var user = await _context.Users.FindAsync(id);
            if (user == null) return false;

            user.FullName = dto.FullName;
            user.Bio = dto.Bio;
            user.Gender = dto.Gender;
            user.BirthDate = dto.BirthDate;
            user.PhotoUrl = dto.PhotoUrl;

            await _context.SaveChangesAsync();
            return true;
        }

        public async Task<bool> PatchAsync(string id, PatchUserDto dto)
        {
            var user = await _context.Users.FindAsync(id);
            if (user == null) return false;

            if (dto.Bio != null) user.Bio = dto.Bio;
            if (dto.PhotoUrl != null) user.PhotoUrl = dto.PhotoUrl;

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
                Id = Guid.NewGuid().ToString(),
                Username = dto.Username,
                Bio = dto.Bio,
                Gender = dto.Gender,
                BirthDate = dto.BirthDate,
                PhotoUrl = dto.PhotoUrl,
                PasswordHash = BCrypt.Net.BCrypt.HashPassword((string)dto.Password)
            };

            _context.Users.Add(user);
            await _context.SaveChangesAsync();

            var userDto = new UserDto
            {
                Id = user.Id,
                Username = user.Username,
                Bio = user.Bio,
                Gender = user.Gender,
                BirthDate = user.BirthDate,
                PhotoUrl = user.PhotoUrl
            };

            return (true, "Пользователь создан", userDto);
        }
    }
}
