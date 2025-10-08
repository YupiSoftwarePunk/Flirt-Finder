using Server.Models;

namespace Server.Repositories.Interfaces
{
    public interface IUserRepository
    {
        Task CreateAsync(User user);
        Task<User> GetByUsernameAsync(string username);

        Task<User> GetByIdAsync(string id);
        Task UpdateAsync(User user);
    }
}
