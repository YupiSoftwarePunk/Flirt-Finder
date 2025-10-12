using Microsoft.AspNetCore.Authentication.JwtBearer;
using Microsoft.EntityFrameworkCore;
using Microsoft.IdentityModel.Tokens;
using Server.Data;
using Server.Repositories;
using Server.Repositories.Interfaces;
using System.Security.Cryptography;
using Microsoft.AspNetCore.Authentication.JwtBearer;
using System.Text;

namespace Server
{
    public class Program
    {
        public static void Main(string[] args)
        {
            DotNetEnv.Env.Load();

            //string skey = "usr-256";
            //var key = new SymmetricSecurityKey(Encoding.UTF8.GetBytes(skey));
            //var credentials = new SigningCredentials(key, SecurityAlgorithms.HmacSha256Signature);
            //Console.WriteLine("Key: " + key);


            var builder = WebApplication.CreateBuilder(args);

            // Add services to the container.

            builder.Services.AddControllers();
            // Learn more about configuring OpenAPI at https://aka.ms/aspnet/openapi
            builder.Services.AddOpenApi();

            var connectionString = builder.Configuration["ConnectionStrings:DefaultConnection"];
            var jwtKey = builder.Configuration["Jwt:Key"];

            builder.Services.AddDbContext<AppDbContext>(options =>
            options.UseNpgsql(connectionString));

            builder.Services.AddAuthentication(options =>
            {
                options.DefaultAuthenticateScheme = JwtBearerDefaults.AuthenticationScheme;
                options.DefaultChallengeScheme = JwtBearerDefaults.AuthenticationScheme;
            })
            .AddJwtBearer(options =>
            {
            options.TokenValidationParameters = new TokenValidationParameters
            {
                ValidateIssuer = true,
                ValidateAudience = true,
                ValidateLifetime = true,
                ValidateIssuerSigningKey = true,
                ValidIssuer = builder.Configuration["Jwt:Issuer"],
                ValidAudience = builder.Configuration["Jwt:Audience"],
                IssuerSigningKey = new SymmetricSecurityKey(Encoding.UTF8.GetBytes(builder.Configuration["Jwt:Key"]))
            };
            });

            builder.Services.AddControllers();

            var app = builder.Build();

            // Configure the HTTP request pipeline.
            if (app.Environment.IsDevelopment())
            {
                app.MapOpenApi();
            }

            app.UseAuthentication();
            //app.UseHttpsRedirection();

            app.UseAuthorization();


            app.MapControllers();

            app.Run();
        }
    }
}
