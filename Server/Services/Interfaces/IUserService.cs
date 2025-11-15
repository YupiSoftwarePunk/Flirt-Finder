using Microsoft.EntityFrameworkCore;
using Server.Data;
using Server.DTOs;
using Server.Models;
using Server.Repositories;

namespace Server.Services.Interfaces
{
    public interface IUserService
    {
        Task<UserProfileDto> GetByIdAsync(int id);
        Task<bool> UpdateAsync(int id, UpdateUserDto dto);
        Task<bool> PatchAsync(int id, PatchUserDto dto);
        Task<(bool Success, string Message, UserDto User)> CreateAsync(CreateUserDto dto);

        Task<IEnumerable<UserProfileDto>> GetProfilesAsync(int excludeUserId, string gender);
        Task<User?> GetByLoginAsync(string login);
    }
}