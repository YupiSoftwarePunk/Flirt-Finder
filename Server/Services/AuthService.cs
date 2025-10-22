using Microsoft.EntityFrameworkCore;
using Microsoft.IdentityModel.Tokens;
using Server.DTOs;
using Server.Models;
using Server.Repositories.Interfaces;
using Server.Services.Interfaces;
using System.IdentityModel.Tokens.Jwt;
using System.Security.Claims;
using System.Text;
using Server.Data;


namespace Server.Services
{
    public class AuthService : IAuthService
    {
        private readonly IUserRepository _userRepository;
        private readonly IConfiguration _configuration;

        public AuthService(IUserRepository userRepository, IConfiguration configuration)
        {
            _userRepository = userRepository;
            _configuration = configuration;
        }


        public async Task<AuthResultDto> Register(RegisterDto dto)
        {
            var existingUser = await _userRepository.GetByLoginAsync(dto.Login);
            if (existingUser != null)
            {
                return new AuthResultDto { Success = false, Message = "Пользователь уже существует" };
            }

            var user = new User
            {
                Login = dto.Login,
                Password = BCrypt.Net.BCrypt.HashPassword(dto.Password),
                Username = "noname",
                Bio = "nohobby",
                Gender = "nogen",
                Age = 16,
                City = "nocity"
            };

            await _userRepository.CreateAsync(user);

            return new AuthResultDto { Success = true, Message = "Регистрация успешна" };
        }

        public async Task<string> Login(LoginDto dto)
        {
            var user = await _userRepository.GetByLoginAsync(dto.Login);
            if (user == null || !BCrypt.Net.BCrypt.Verify(dto.Password, user.Password))
            {
                return null;
            }

            var tokenHandler = new JwtSecurityTokenHandler();
            var key = Encoding.UTF8.GetBytes(_configuration["Jwt:Key"]);

            var tokenDescriptor = new SecurityTokenDescriptor
            {
                Subject = new ClaimsIdentity(new[]
                {
                    new Claim(ClaimTypes.NameIdentifier, user.Id.ToString()),
                    new Claim(ClaimTypes.Name, user.Username)
                }),
                Expires = DateTime.UtcNow.AddDays(7),
                SigningCredentials = new SigningCredentials(
                    new SymmetricSecurityKey(key),
                    SecurityAlgorithms.HmacSha256Signature)
            };

            var token = tokenHandler.CreateToken(tokenDescriptor);
            return tokenHandler.WriteToken(token);
        }
    }
}
