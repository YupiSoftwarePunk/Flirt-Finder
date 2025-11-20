using Server.Models;
using Microsoft.EntityFrameworkCore;
using System.Collections.Generic;

namespace Server.Data
{
    public class AppDbContext : DbContext
    {
        public AppDbContext(DbContextOptions<AppDbContext> options) : base(options) { }

        public DbSet<Message> Messages { get; set; }
        public DbSet<User> Users { get; set; }
        public DbSet<MessageReport> MessageReports { get; set; }
        public DbSet<Photo> Photos { get; set; }
        public DbSet<Reaction> LikesDislikes { get; set; }

        public DbSet<City> Cities { get; set; }
    }
}
