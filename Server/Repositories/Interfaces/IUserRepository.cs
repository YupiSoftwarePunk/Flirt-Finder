using Server.Models;

namespace Server.Repositories.Interfaces
{
    public interface IUserRepository
    {
        Task CreateAsync(User user);
        Task<User?> GetByLoginAsync(string login);

        Task<User?> GetByIdAsync(int id);
        Task UpdateAsync(User user);
    }
}
