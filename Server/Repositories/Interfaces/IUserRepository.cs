using Server.Models;

namespace Server.Repositories.Interfaces
{
    public interface IUserRepository
    {
        Task CreateAsync(User user);
        Task GetByUsernameAsync(string username);
    }
}
