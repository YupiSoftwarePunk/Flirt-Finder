using Server.Models;

namespace Server.Repositories.Interfaces
{
    public interface IReactionRepository
    {
        Task<Reaction?> GetAsync(int userId, int likedBy);
        Task SaveAsync(Reaction reaction);
        Task<bool> HasMutualLikeAsync(int currentUserId, int targetUserId);
    }
}
