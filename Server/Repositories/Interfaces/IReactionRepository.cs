using Server.Models;

namespace Server.Repositories.Interfaces
{
    public class IReactionRepository
    {
        Task<Reaction?> GetAsync(int userId, int likedBy);
        Task SaveAsync(Reaction reaction);
    }
}
