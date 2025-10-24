using Server.DTOs;
using Server.Models;

namespace Server.Services.Interfaces
{
    public interface ICityService
    {
        Task<bool> CityExistsAsync(string name);
    }
}
