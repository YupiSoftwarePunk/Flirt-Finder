using Server.DTOs;
using Server.Models;
using Server.Repositories.Interfaces;
using Server.Services.Interfaces;

namespace Server.Services
{
    public class ReactionService : IReactionService
    {
        private readonly IReactionRepository _repository;

        public ReactionService(IReactionRepository repository)
        {
            _repository = repository;
        }

        public async Task<bool> SaveReactionAsync(int currentUserId, ReactionDto dto)
        {
            var reaction = new Reaction
            {
                UserId = dto.TargetUserId,
                LikedBy = currentUserId,
                UserReaction = dto.Reaction
            };

            await _repository.SaveAsync(reaction);
            return true;
        }
    }
}
