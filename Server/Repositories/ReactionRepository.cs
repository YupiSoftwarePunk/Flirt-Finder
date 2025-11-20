using Microsoft.EntityFrameworkCore;
using Server.Data;
using Server.Models;
using Server.Repositories.Interfaces;

namespace Server.Repositories
{
    public class ReactionRepository : IReactionRepository
    {
        private readonly AppDbContext _context;

        public ReactionRepository(AppDbContext context)
        {
            _context = context;
        }

        public async Task<Reaction?> GetAsync(int userId, int likedBy)
        {
            return await _context.Reactions
                .FirstOrDefaultAsync(r => r.UserId == userId && r.LikedBy == likedBy);
        }

        public async Task SaveAsync(Reaction reaction)
        {
            var existing = await GetAsync(reaction.UserId, reaction.LikedBy);
            if (existing == null)
            {
                _context.Reactions.Add(reaction);
            }
            else
            {
                existing.UserReaction = reaction.UserReaction;
                _context.Reactions.Update(existing);
            }
            await _context.SaveChangesAsync();
        }
    }
}
