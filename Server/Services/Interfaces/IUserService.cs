using Microsoft.EntityFrameworkCore;
using Server.Data;
using Server.DTOs;
using Server.Models;
using Server.Repositories;

namespace Server.Services.Interfaces
{
    public interface IUserService
    {
        Task<UserDto> GetByIdAsync(string id);
        Task<bool> UpdateAsync(string id, UpdateUserDto dto);
        Task<bool> PatchAsync(string id, PatchUserDto dto);
        Task<(bool Success, string Message, UserDto User)> CreateAsync(CreateUserDto dto);
    }
}