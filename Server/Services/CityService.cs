using Microsoft.EntityFrameworkCore;
using Server.Data;
using Server.Services.Interfaces;

namespace Server.Services
{
    public class CityService : ICityService
    {
        private readonly AppDbContext _context;

        public CityService(AppDbContext context)
        {
            _context = context;
        }

        public async Task<bool> CityExistsAsync(string name)
        {
            return await _context.Cities.AnyAsync(c => c.Name == name);
        }
    }
}
