using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Mvc;
using Server.DTOs;
using Server.Services;
using System.Security.Claims;

namespace Server.Controllers
{
    [Route("api/[controller]")]
    [ApiController]
    public class ReactionsController : ControllerBase
    {
        private readonly ReactionService _reactionService;

        public ReactionsController(ReactionService reactionService)
        {
            _reactionService = reactionService;
        }

        [Authorize]
        [HttpPost]
        public async Task<IActionResult> SaveReaction([FromBody] ReactionDto dto)
        {
            var userIdStr = User.FindFirstValue(ClaimTypes.NameIdentifier);
            if (!int.TryParse(userIdStr, out var currentUserId))
                return Unauthorized();

            await _reactionService.SaveReactionAsync(currentUserId, dto);
            return Ok(new { message = "Реакция сохранена" });
        }


        [Authorize]
        [HttpGet("mutual")]
        public async Task<IActionResult> CheckMutualLike([FromQuery] int targetUserId)
        {
            var userIdStr = User.FindFirstValue(ClaimTypes.NameIdentifier);
            if (!int.TryParse(userIdStr, out var currentUserId))
                return Unauthorized();

            var mutualLike = await _reactionService.CheckMutualLikeAsync(currentUserId, targetUserId);

            return Ok(new { mutualLike });
        }
    }
}
