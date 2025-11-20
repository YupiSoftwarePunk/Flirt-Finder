using Server.DTOs;
using Server.Repositories.Interfaces;

namespace Server.Services.Interfaces
{
    public interface IReactionService
    {
        Task<bool> SaveReactionAsync(int currentUserId, ReactionDto dto);



    }
}
