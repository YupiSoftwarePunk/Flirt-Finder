using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Mvc;
using Server.DTOs;
using Server.Models;
using Server.Repositories.Interfaces;
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
        private readonly IUserRepository _userRepository;

        public UsersController(IUserService userService, IPhotoService photoService, IUserRepository user)
        {
            _userService = userService;
            _photoService = photoService;
            _userRepository = user;
        }

        [Authorize]
        [HttpGet("me")]
        public async Task<IActionResult> GetCurrentUser()
        {
            var userIdStr = User.FindFirst(ClaimTypes.NameIdentifier)?.Value;
            if (!int.TryParse(userIdStr, out var userId)) return Unauthorized();

            var user = await _userService.GetByIdAsync(userId);
            var photo = await _photoService.GetPhotoUrlByUserIdAsync(userId);

            var dto = new UserProfileDto
            {
                FullName = user.Username,
                Age = user.Age,
                City = user.City,
                Bio = user.Bio,
                Gender = user.Gender,
                Login = user.Login,
                PhotoUrl = photo != null ? $"http://localhost:5002/api/users/me/photo" : null
            };

            return Ok(user);
        }


        [Authorize]
        [HttpPut("me/basic")]
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

            if (photo.Content == null)
                return NotFound("Фото не найдено");
            var contentType = string.IsNullOrEmpty(photo.ContentType) ? "image/jpeg" : photo.ContentType;

            return File(photo.Content, contentType);
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
        [HttpPut("me/full")]
        public async Task<IActionResult> UpdateUser([FromBody] UpdateUserDto dto)
        {
            try
            {
                var userIdStr = User.FindFirstValue(ClaimTypes.NameIdentifier);
                if (!int.TryParse(userIdStr, out var userId)) return Unauthorized();

                var user = await _userRepository.GetByIdAsync(userId);
                if (user == null) return NotFound();

                user.Username = dto.FullName;
                user.Age = dto.Age;
                user.City = dto.City;
                user.Bio = dto.Bio;
                user.Gender = dto.Gender;

                var result = await _userService.UpdateAsync(userId, dto);
                return result ? NoContent() : BadRequest("Ошибка обновления");
            }
            catch(Exception ex)
            {
                Console.WriteLine($"Ошибка при обновлении пользователя: {ex.Message}");
                return StatusCode(500, "Внутренняя ошибка сервера");
            }
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


        [Authorize]
        [HttpPost("me/photo")]
        public async Task<IActionResult> UploadPhoto(IFormFile file)
        {
            var userIdStr = User.FindFirstValue(ClaimTypes.NameIdentifier);
            if (!int.TryParse(userIdStr, out var userId)) return Unauthorized();

            if (file == null || file.Length == 0)
                return BadRequest("Файл не получен");

            // Сохраняем фото
            await _photoService.SavePhotoAsync(userId, file);

            // Загружаем пользователя
            var user = await _userRepository.GetByIdAsync(userId);
            if (user == null) return NotFound("Пользователь не найден");

            return Ok("Фото загружено");
        }


        [Authorize]
        [HttpGet("profiles")]
        public async Task<IActionResult> GetProfiles([FromQuery] string login)
        {
            var currentUser = await _userService.GetByLoginAsync(login);
            if (currentUser == null)
            {
                Console.WriteLine($"Пользователь с логином '{login}' не найден");
                return NotFound("Пользователь с таким логином не найден");
            }

            var oppositeGender = currentUser.Gender == "Мужской" ? "Женский" : "Мужской";

            var profiles = await _userService.GetProfilesAsync(currentUser.Id, oppositeGender);
            return Ok(profiles);
        }


        //[Authorize]
        //[HttpGet("me/photo")]
        //public async Task<IActionResult> GetMyPhoto()
        //{
        //    var userIdStr = User.FindFirstValue(ClaimTypes.NameIdentifier);
        //    if (!int.TryParse(userIdStr, out var userId))
        //        return Unauthorized();

        //    var photo = await _photoService.GetPhotoByUserIdAsync(userId);
        //    if (photo == null || string.IsNullOrEmpty(photo.Url))
        //        return NotFound("Фото не найдено");

        //    var filePath = photo.Url;
        //    if (!System.IO.File.Exists(filePath))
        //        return NotFound("Файл изображения не найден");

        //    var contentType = string.IsNullOrEmpty(photo.ContentType) ? "image/jpeg" : photo.ContentType;
        //    var fileBytes = await System.IO.File.ReadAllBytesAsync(filePath);
        //    return File(fileBytes, contentType);
        //}
    }
}
