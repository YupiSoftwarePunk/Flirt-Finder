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

        public UsersController(IUserService userService)
        {
            _userService = userService;
        }


        [Authorize]
        [HttpGet("me")]
        public async Task<IActionResult> GetCurrentUser()
        {
            var userId = User.FindFirst(ClaimTypes.NameIdentifier)?.Value;
            var user = await _userService.GetByIdAsync(userId);
            return Ok(user);
        }


        [Authorize]
        [HttpPut("me")]
        public async Task<IActionResult> UpdateProfile(UpdateUserDto dto)
        {
            var userId = User.FindFirst(ClaimTypes.NameIdentifier)?.Value;
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
        [HttpGet("{id}")]
        public async Task<IActionResult> GetUserById(string id)
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
            var userId = User.FindFirstValue(ClaimTypes.NameIdentifier);
            var result = await _userService.UpdateAsync(userId, dto);
            return result ? NoContent() : BadRequest("Ошибка обновления");
        }



        // Частичное обновление анкеты
        [Authorize]
        [HttpPatch("me")]
        public async Task<IActionResult> PatchUser([FromBody] PatchUserDto dto)
        {
            var userId = User.FindFirstValue(ClaimTypes.NameIdentifier);
            var result = await _userService.PatchAsync(userId, dto);
            return result ? NoContent() : BadRequest("Ошибка обновления");
        }
    }
}
