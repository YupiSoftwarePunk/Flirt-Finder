using Microsoft.EntityFrameworkCore;
using Server.Data;
using Server.DTOs;
using Server.Models;
using Server.Services.Interfaces;
using System;

namespace Server.Services
{
    public class UserService : IUserService
    {
        private readonly AppDbContext _context;
        private readonly IPhotoService _photoService;
        private IEnumerable<object> photos;

        public UserService(AppDbContext context, IPhotoService photoService)
        {
            _context = context;
            _photoService = photoService;
        }


        public async Task<UserProfileDto> GetByIdAsync(int id)
        {
            var user = await _context.Users.FindAsync(id);

            if (user == null) return null;

            var photos = await _context.Photos
            .Where(p => p.UserId == id)
            .ToListAsync();

            return new UserProfileDto
            {
                User = new UserDto
                {
                    Id = user.Id,
                    Username = user.Username,
                    Bio = user.Bio,
                    Gender = user.Gender,
                    Login = user.Login,
                    City = user.City,
                    Age = user.Age
                },
                Photos = photos.Select(p => new PhotoDto
                {
                    Id = p.Id,
                    Url = p.Url,
                    UserId = user.Id
                }).ToList()
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
            user.Username = dto.Username;

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
                    Username = user.Username,
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
