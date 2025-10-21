using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Mvc;
using Server.DTOs;
using Server.Services;
using Server.Services.Interfaces;
using System.Security.Claims;

namespace Server.Controllers
{
    [Route("api/[controller]")]
    [ApiController]
    public class UsersController : ControllerBase
    {
        private readonly IUserService _userService;
        private readonly IPhotoService _photoService;

        public UsersController(IUserService userService, IPhotoService photoService)
        {
            _userService = userService;
            _photoService = photoService;
        }

        [Authorize]
        [HttpGet("me")]
        public async Task<IActionResult> GetCurrentUser()
        {
            var userIdStr = User.FindFirst(ClaimTypes.NameIdentifier)?.Value;
            if (!int.TryParse(userIdStr, out var userId)) return Unauthorized();

            var user = await _userService.GetByIdAsync(userId);
            return Ok(user);
        }


        [Authorize]
        [HttpPut("me")]
        public async Task<IActionResult> UpdateProfile(UpdateUserDto dto)
        {
            var userIdStr = User.FindFirst(ClaimTypes.NameIdentifier)?.Value;
            if (!int.TryParse(userIdStr, out var userId)) return Unauthorized();

            await _userService.UpdateAsync(userId, dto);
            return NoContent();
        }



        [HttpGet("{login}/photo")]
        public async Task<IActionResult> GetPhoto(string login)
        {
            var photo = await _photoService.GetPhotoByLoginAsync(login);
            return File(photo.Content, photo.ContentType);
        }



        // Получение пользователя по ID
        [Authorize]
        [HttpGet("{id:int}")]
        public async Task<IActionResult> GetUserById(int id)
        {
            var user = await _userService.GetByIdAsync(id);
            return user != null ? Ok(user) : NotFound();
        }



        // Получение ID текущего пользователя
        [Authorize]
        [HttpGet("me/id")]
        public IActionResult GetCurrentUserId()
        {
            var userId = User.FindFirstValue(ClaimTypes.NameIdentifier);
            return Ok(new { userId });
        }



        // Обновление анкеты полностью
        [Authorize]
        [HttpPut("me")]
        public async Task<IActionResult> UpdateUser([FromBody] UpdateUserDto dto)
        {
            var userIdStr = User.FindFirstValue(ClaimTypes.NameIdentifier);
            if (!int.TryParse(userIdStr, out var userId)) return Unauthorized();

            var result = await _userService.UpdateAsync(userId, dto);
            return result ? NoContent() : BadRequest("Ошибка обновления");
        }



        // Частичное обновление анкеты
        [Authorize]
        [HttpPatch("me")]
        public async Task<IActionResult> PatchUser([FromBody] PatchUserDto dto)
        {
            var userIdStr = User.FindFirstValue(ClaimTypes.NameIdentifier);
            if (!int.TryParse(userIdStr, out var userId)) return Unauthorized();

            var result = await _userService.PatchAsync(userId, dto);
            return result ? NoContent() : BadRequest("Ошибка обновления");
        }
    }
}
