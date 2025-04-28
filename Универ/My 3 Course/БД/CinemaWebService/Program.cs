using CinemaWebService.Data;
using Microsoft.EntityFrameworkCore;  
using Microsoft.OpenApi.Models;  
using CinemaWebService.Services;
using MongoDB.Driver;


//postgres

// var builder = WebApplication.CreateBuilder(args);
//
// builder.Services.AddDbContext<ApplicationDbContext>(options =>
//     options.UseNpgsql(builder.Configuration.GetConnectionString("DefaultConnection")));
//
// builder.Services.AddControllers();
// builder.Services.AddTransient<MigrationService>();
//
// builder.Services.AddEndpointsApiExplorer();
// builder.Services.AddSwaggerGen();
//
// var app = builder.Build();
//
// if (app.Environment.IsDevelopment())
// {
//     app.UseSwagger();
//     app.UseSwaggerUI();
// }
//
// using (var scope = app.Services.CreateScope())
// {
//     var migrationService = scope.ServiceProvider.GetRequiredService<MigrationService>();
//     
//     // Вызываем миграцию данных
//     await migrationService.MigrateAllAsync();
// }
//
//
// app.UseAuthorization();
//
// app.MapControllers();
//
// app.Run();
//
//
//
//
//



var builder = WebApplication.CreateBuilder(args);

builder.Services.AddSingleton<IMongoClient>(sp =>
{
    var mongoConnectionString = builder.Configuration.GetConnectionString("MongoDbConnection");
    return new MongoClient(mongoConnectionString);
});

builder.Services.AddScoped(sp =>
{
    var client = sp.GetRequiredService<IMongoClient>();
    return client.GetDatabase("cinema_mongo_db");
});

builder.Services.AddControllers();
builder.Services.AddEndpointsApiExplorer();
builder.Services.AddSwaggerGen();

var app = builder.Build();


if (app.Environment.IsDevelopment())
{
    app.UseSwagger();
    app.UseSwaggerUI();
}

app.UseAuthorization();
app.MapControllers();
app.Run();





//for migrations
//
// using CinemaWebService.Data;
// using CinemaWebService.Services;
// using Microsoft.EntityFrameworkCore;
// using MongoDB.Driver;
//
// var builder = WebApplication.CreateBuilder(args);
//
// // Регистрация сервисов
// builder.Services.AddDbContext<ApplicationDbContext>(options =>
//     options.UseNpgsql(builder.Configuration.GetConnectionString("DefaultConnection")));
//
// builder.Services.AddSingleton<IMongoClient>(sp =>
// {
//     var mongoConnectionString = builder.Configuration.GetConnectionString("MongoDbConnection");
//     return new MongoClient(mongoConnectionString);
// });
//
// builder.Services.AddScoped(sp =>
// {
//     var client = sp.GetRequiredService<IMongoClient>();
//     return client.GetDatabase("cinema_mongo_db");
// });
//
// builder.Services.AddControllers();
// builder.Services.AddTransient<MigrationService>();
//
// builder.Services.AddEndpointsApiExplorer();
// builder.Services.AddSwaggerGen();
//
// var app = builder.Build();
//
// if (app.Environment.IsDevelopment())
// {
//     app.UseSwagger();
//     app.UseSwaggerUI();
// }
//
// // Выполнение миграции при запуске приложения
// using (var scope = app.Services.CreateScope())
// {
//     var migrationService = scope.ServiceProvider.GetRequiredService<MigrationService>();
//     await migrationService.MigrateAllAsync();
// }
//
// app.UseAuthorization();
// app.MapControllers();
// app.Run();